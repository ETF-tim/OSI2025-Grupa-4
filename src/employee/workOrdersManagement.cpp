#include "../../include/employee/workOrdersManagement.hpp"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "../../include/common/models/WorkOrder.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/employeesManagement.hpp"
#include "../../include/employee/partsManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"

std::string WorkOrderManager::statusToString (WorkOrderStatus status) {
    switch (status) {
        case WorkOrderStatus::IN_DIAGNOSTICS:
            return "IN_DIAGNOSTICS";
        case WorkOrderStatus::IN_REPAIR:
            return "IN_REPAIR";
        case WorkOrderStatus::WAITING_FOR_PARTS:
            return "WAITING_FOR_PARTS";
        case WorkOrderStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "UNKNOWN";
    }
}

std::string WorkOrderManager::partsToString (const std::map<int, int>& parts) {
    if (parts.empty ()) return "";
    std::string result;
    for (const auto& p : parts) {
        if (!(result.empty ())) result += ';';
        result += std::to_string (p.first) + ':' + std::to_string (p.second);
    }
    return result;
}

std::map<int, int> WorkOrderManager::stringToParts (const std::string& s) {
    std::map<int, int> parts;
    if (s.empty ()) return parts;

    size_t start = 0;
    size_t end = s.find (';');

    while (end != std::string::npos) {
        std::string pair = s.substr (start, end - start);
        size_t colonPos = pair.find (':');
        if (colonPos != std::string::npos) {
            int partId = std::stoi (pair.substr (0, colonPos));
            int quantity = std::stoi (pair.substr (colonPos + 1));
            parts[partId] = quantity;
        }
        start = end + 1;
        end = s.find (';', start);
    }

    // Process last pair
    std::string pair = s.substr (start);
    size_t colonPos = pair.find (':');
    if (colonPos != std::string::npos) {
        int partId = std::stoi (pair.substr (0, colonPos));
        int quantity = std::stoi (pair.substr (colonPos + 1));
        parts[partId] = quantity;
    }

    return parts;
}

std::string formatTimestamp (const std::string& timestampStr) {
    if (timestampStr == "0" || timestampStr.empty ()) return "Nije završeno";

    std::time_t timestamp = std::stoll (timestampStr);
    std::tm* timeinfo = std::localtime (&timestamp);

    std::ostringstream oss;
    oss << std::put_time (timeinfo, "%d.%m.%Y %H:%M:%S");
    return oss.str ();
}

WorkOrderManager::WorkOrderManager (ReceiptOrderManager& receiptOrderManager, PartManager& partManager)
    : receiptOrderManager (receiptOrderManager), partManager (partManager) {}

void WorkOrderManager::createWorkOrder (int technicianID) {
    // Opening CSV file
    CSVData workOrders;
    CSVData receiptOrders;
    CSVData users;

    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog prijemnog naloga";
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = workOrders.get_row (0);
    workOrders.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (workOrders);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog radnog naloga";
        return;
    }  //------------------

    // Print list of free receipt orders
    receiptOrderManager.listFreeReceiptOrders ();

    // ----------------

    int temptReceiptOrderID;
    std::cout << "Unesite ID prijemnog naloga za koji kreirate radni nalog: ";
    std::cin >> temptReceiptOrderID;
    std::cin.ignore ();  // Clear newline character from input buffer
    // ----------------

    // Check if receipt order exists
    if (!receiptOrderManager.searchForReceiptOrder (temptReceiptOrderID)) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Check if receipt order is free
    if (!receiptOrderManager.isReceiptOrderFree (temptReceiptOrderID)) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije slobodan." << std::endl;
        return;
    }  //------------------

    std::string imeKlijenta;
    std::string prezimeKlijenta;
    std::string IMEIUredjaja;

    int foundReceiptOrderIndex;
    int foundUserIndex;
    bool receiptFound = false;
    bool userFound = false;

    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno otvaranje fajla sa prijemnim nalozima";
        return;
    }
    try {
        users = CSVData ("./data/users.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno otvaranje fajla sa korisnicima";
        return;
    }

    for (int rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == temptReceiptOrderID) {
            foundReceiptOrderIndex = rowIndex;
            receiptFound = true;
            break;
        }
    }
    for (int rowIndex = 1; rowIndex < users.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (users.get_value (rowIndex, 0)) == std::stoi (receiptOrders.get_value (foundReceiptOrderIndex, 1))) {
            foundUserIndex = rowIndex;
            userFound = true;
            break;
        }
    }
    IMEIUredjaja = receiptOrders.get_value (foundReceiptOrderIndex, 2);
    imeKlijenta = users.get_value (foundUserIndex, 1);
    prezimeKlijenta = users.get_value (foundUserIndex, 2);

    if (!receiptFound || !userFound) {
        std::cerr << "Greska pri pronalazenju podataka o klijentu." << std::endl;
        return;
    }

    std::cout << "Ime klijenta:" << imeKlijenta << std::endl;
    std::cout << "Prezime klijenta:" << prezimeKlijenta << std::endl;
    std::cout << "IMEI uredjaja klijenta:" << IMEIUredjaja << std::endl;

    std::cout << "Da li su ovi podaci tacni? (d/n): ";
    char confirm;
    std::cin >> confirm;
    std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');

    if (confirm != 'd' && confirm != 'D') {
        std::cout << "Kreiranje radnog naloga prekinuto." << std::endl;
        return;
    }

    std::cout << "Promjena statusa prijemnog naloga sa slobodan na zauset" << std::endl;
    receiptOrderManager.changeReceiptOrderStatus (temptReceiptOrderID, 0);  // change receipt order status from free to not free
    // Set initial status to IN_DIAGNOSTICS when starting work on device
    WorkOrderStatus currentStatus = WorkOrderStatus::IN_DIAGNOSTICS;
    std::cout << "\n Radni nalog je kreiran. Status: IN_DIAGNOSTICS" << std::endl;
    std::cout << "Tehnicar pocinje sa dijagnostikom uredjaja...\n" << std::endl;

    std::time_t startDate = std::time (nullptr);  // Current date/time
    std::time_t endDate = 0;
    double servicePrice = 0;  // Will be updated when work is COMPLETED
    std::string comment;
    int response;
    std::cout << "Da li zelite uneti komentar: (d=1/n=2)" << std::endl;
    do {
        std::cout << "Unesite odgovor: ";
        std::cin >> response;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');  // Clear newline character from input buffer
    } while (response < 1 || response > 2);

    if (response == 1) {
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        std::cout << "Unesite komentar:" << std::endl;
        std::getline (std::cin, comment);
    }

    workOrders.add_row (header, 0);  // Re-add header row
    workOrders.add_row ({std::to_string (tempId), std::to_string (temptReceiptOrderID), statusToString (currentStatus), comment,
                         std::to_string (startDate), std::to_string (endDate), std::to_string (technicianID), "",
                         std::to_string (servicePrice)});

    // Write to file
    workOrders.write_data ("./data/workOrders.csv");

    std::cout << "\nUspjesno kreiran radni nalog!" << std::endl;
    std::cout << "ID: " << tempId << std::endl;
    std::cout << "Status: " << statusToString (currentStatus) << std::endl;
}

void WorkOrderManager::listWorkOrders () {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste radnih naloga";
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = workOrders.get_row (0);
    workOrders.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by workOrder ID in ascending order
    workOrders.sort_by_col (0, CSVData::ASC);

    // Re-add header row
    workOrders.add_row (header, 0);  // Re-add header row
    //------------------

    // Print receipt orders list
    std::cout << "----- LISTA RADNIH NALOGA -----" << std::endl;
    workOrders.print_csv_data ();
}

void WorkOrderManager::updateWorkOrders () {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno azuriranje  radnog naloga";
        return;
    }

    // Print list of free receipt orders
    listWorkOrders ();
    std::vector<std::string> header = workOrders.get_row (0);
    workOrders.delete_row (0);  // Remove header row
    // ----------------

    int temptWorkOrderID;
    bool found = false;
    int foundIndex = -1;
    std::cout << "Unesite ID radnog naloga koji zelite azurirati: ";
    std::cin >> temptWorkOrderID;
    std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');  // Clear newline character from input buffer
    // ----------------

    for (int rowIndex = 0; rowIndex < workOrders.rows (); rowIndex++) {
        if (std::stoi (workOrders.get_value (rowIndex, 0)) == temptWorkOrderID) {
            std::cout << "Radni nalog pronadjen u csv fajlu." << std::endl;
            found = true;
            foundIndex = rowIndex;
            break;
        }
    }

    if (!found) {
        std::cerr << "Radni nalog nije pronadjen." << std::endl;
        return;
    }
    // Select which parts will be used during the repair
    std::map<int, int> usedParts;
    std::string currentParts = workOrders.get_value (foundIndex, 7);
    usedParts = stringToParts (currentParts);

    std::cout << "Trenutni dijelovi u popravci: " << currentParts << std::endl;

    CSVData parts;
    std::string NazivUredjaja;
    std::string kolicinaNaStanju;
    std::string kriticnaKolicina;
    std::string cijenaDijela;

    int foundPartIndex;
    bool partFound = false;

    try {
        parts = CSVData ("./data/parts.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno otvaranje fajla sa dijelovima";
        return;
    }

    for (const auto& [partId, qty] : usedParts) {
        for (int rowIndex = 1; rowIndex < parts.rows (); rowIndex++) {  // Start from 1 to skip header row
            if (std::stoi (parts.get_value (rowIndex, 0)) == partId) {
                foundPartIndex = rowIndex;
                partFound = true;
                break;
            }
        }

        if (!partFound) {
            std::cout << "Dio " << partId << " ne postoji u parts.csv." << std::endl;
        } else {
            NazivUredjaja = parts.get_value (foundPartIndex, 1);
            kolicinaNaStanju = parts.get_value (foundPartIndex, 2);
            kriticnaKolicina = parts.get_value (foundPartIndex, 3);
            cijenaDijela = parts.get_value (foundPartIndex, 4);

            std::cout << "Dio:" << NazivUredjaja << "," << " Kolicina na stanju:" << kolicinaNaStanju << ","
                      << " Kriticna kolicina:" << kriticnaKolicina << "," << " Cijena dijela:" << cijenaDijela << ","
                      << "Uzeta kolicina dijela u popravci:" << "," << qty << std::endl;
        }
        partFound = false;
    }

    std::cout << "Azuriranje dijelova u popravci uredjaja." << std::endl;
    std::cout << "Ako nema dodavanja novih dijelova, u sljedecoj poruci unesite n/N" << std::endl;

    while (true) {
        std::cout << "Dodaj dio u popravku ili promijeni kolicinu nekog dijela? (d/n):";
        char choice;
        std::cin >> choice;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');  // Clear newline from buffer

        if (choice == 'n' || choice == 'N') break;  // If input is 'n', exit from the loop

        int partId, quantity;

        std::cout << "Unesite id dijela koji zelite dodati/promijeniti: ";
        std::cin >> partId;
        std::cout << "Unesite pozitivnu kolicinu za uzimanje dijelova,\n";
        std::cout << "ili negativnu za vracanje dijelova na skladiste.\n";
        std::cin >> quantity;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');  // Clear newline from buffer

        // Check 1: does part exist
        if (!partManager.searchForPart (partId)) {
            std::cout << "Dio sa unesenim ID-em nije pronadjen." << std::endl;
            continue;
        }

        // Check 2: is part available with this quantity
        if (quantity > 0) {
            // Uzimanje dijelova sa skladišta
            if (!partManager.isPartAvailable (partId, quantity)) {
                std::cout << "Nema dovoljno dijelova u skladistu." << std::endl;
                continue;
            }

            partManager.decreasePartStock (partId, quantity);
            usedParts[partId] += quantity;
        } else if (quantity < 0) {
            int absQty = std::abs (quantity);

            // Provjera da li radni nalog ima toliko dijelova
            if (usedParts[partId] < absQty) {
                std::cout << "Ne mozete vratiti vise dijelova nego sto je uzeto." << std::endl;
                continue;
            }

            partManager.increasePartStock (partId, absQty);
            usedParts[partId] -= absQty;
        } else {
            std::cout << "Kolicina ne moze biti 0." << std::endl;
        }
    }

    std::cout << "\n----- Nova lista dijelova u popravci -----" << std::endl;
    for (const auto& [partId, qty] : usedParts) {
        partFound = false;
        for (int rowIndex = 1; rowIndex < parts.rows (); rowIndex++) {
            if (std::stoi (parts.get_value (rowIndex, 0)) == partId) {
                foundPartIndex = rowIndex;
                partFound = true;
                break;
            }
        }

        if (partFound) {
            NazivUredjaja = parts.get_value (foundPartIndex, 1);
            std::cout << "  - Dio: " << NazivUredjaja << " (ID: " << partId << "), Kolicina: " << qty << std::endl;
        }
    }

    WorkOrderStatus currentStatus;
    std::cout << "Izaberite novo stanje radnog naloga, ako zelite promjeniti:" << std::endl;
    std::cout << "1. IN_DIAGNOSTICS" << std::endl;
    std::cout << "2. IN_REPAIR" << std::endl;
    std::cout << "3. WAITING_FOR_PARTS" << std::endl;
    std::cout << "4. COMPLETED" << std::endl;

    int attributeChoice;
    do {
        std::cout << "Unesite broj stanja (1-4): ";
        std::cin >> attributeChoice;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 4);

    switch (attributeChoice) {
        case 1:
            currentStatus = WorkOrderStatus::IN_DIAGNOSTICS;
            break;
        case 2:
            currentStatus = WorkOrderStatus::IN_REPAIR;
            break;
        case 3:
            currentStatus = WorkOrderStatus::WAITING_FOR_PARTS;
            break;
        case 4:
            currentStatus = WorkOrderStatus::COMPLETED;
            break;
    }
    double servicePrice = 0;
    std::string comment;
    int response;
    std::cout << "Da li zelite uneti komentar: (d=1/n=2)" << std::endl;
    do {
        std::cout << "Unesite odgovor: ";
        std::cin >> response;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');  // Clear newline character from input buffer
    } while (response < 1 || response > 2);

    if (response == 1) {
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        std::cout << "Unesite komentar:" << std::endl;
        std::getline (std::cin, comment);
    }

    std::cout << "Da li zelite promijeniti cijenu popravke uredjaja: (d=1/n=2)" << std::endl;
    do {
        std::cout << "Unesite odgovor: ";
        std::cin >> response;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        // Clear newline character from input buffer
    } while (response < 1 || response > 2);

    if (response == 1) {
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        std::cout << "Unesite cijenu:" << std::endl;
        std::cin >> servicePrice;
    }

    std::cout << "Da li je radni nalog zaista zavrsen: (d=1/n=2)" << std::endl;
    do {
        std::cout << "Unesite odgovor: ";
        std::cin >> response;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        // Clear newline character from input buffer
    } while (response < 1 || response > 2);

    if (response == 1) {
        std::time_t endDate = std::time (nullptr);  // Current date/time
        workOrders.set_value (foundIndex, 5, std::to_string (endDate));
        do {
            if (servicePrice <= 0) std::cout << "Unesite finalnu cijenu popravke uredjaja (>0):" << std::endl;
            std::cin >> servicePrice;
        } while (servicePrice <= 0);
    }

    workOrders.set_value (foundIndex, 2, statusToString (currentStatus));
    workOrders.set_value (foundIndex, 3, comment);
    workOrders.set_value (foundIndex, 7, partsToString (usedParts));
    workOrders.set_value (foundIndex, 8, std::to_string (servicePrice));

    workOrders.add_row (header, 0);  // Re-add header row
    std::cout << "\nUspjesno azuriran radni nalog!" << std::endl;
    std::cout << "ID: " << temptWorkOrderID << std::endl;
    std::cout << "Status: " << statusToString (currentStatus) << std::endl;

    workOrders.write_data ("./data/workOrders.csv");
}

void WorkOrderManager::deleteWorkOrder () {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje radnog naloga";
        return;
    }  //------------------

    // Print work orders list
    listWorkOrders ();
    //------------------

    // Choose work order to delete by ID and check if it exists
    // -> Choose work order to delete by ID
    std::cout << "Unesite ID radnog naloga koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool workOrderFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (workOrders.get_value (rowIndex, 0)) == deleteId) {
            workOrderFound = true;
            break;
        }
    }
    if (!workOrderFound) {
        std::cerr << "Radni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete work order from CSV data
    workOrders.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    workOrders.write_data ("./data/workOrders.csv");
    // Print updated work orders list
    listWorkOrders ();
    std::cout << "Uspjesno obrisan radni nalog!" << std::endl;
}

bool WorkOrderManager::searchForWorkOrder (int workOrderId) {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje radnog naloga";
        return false;
    }  //------------------

    // Check if part exists
    for (int rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (workOrders.get_value (rowIndex, 0)) == workOrderId) {
            return true;
        }
    }
    return false;
}

void WorkOrderManager::listCompletedWorkOrders () {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste gotovih radnih naloga";
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = workOrders.get_row (0);
    workOrders.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by receiptOrder ID in ascending order
    workOrders.sort_by_col (0, CSVData::ASC);

    // Re-add header row
    workOrders.add_row (header, 0);  // Re-add header row
    //------------------

    // Filtering free receipt orders (delete where taken status is true)
    for (int rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (workOrders.get_value (rowIndex, 2) != "COMPLETED") {
            workOrders.delete_row (rowIndex);
            rowIndex--;  // Adjust index after deletion
        }
    }
    //------------------

    // Print free receipt orders list
    std::cout << "----- LISTA GOTOVIH RADNIH NALOGA -----" << std::endl;
    workOrders.print_csv_data ();
}

bool WorkOrderManager::isWorkOrderCompleted (int workOrderId) {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno provjeravanje statusa radnog naloga";
        return false;
    }  //------------------

    for (int rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (workOrders.get_value (rowIndex, 0)) == workOrderId) {
            return workOrders.get_value (rowIndex, 2) == "COMPLETED";
        }
    }
    return false;
}

void WorkOrderManager::generateWorkOrderTXTFile (int workOrderId) {
    CSVData employees;
    CSVData workOrders;
    CSVData receiptOrders;
    CSVData users;
    CSVData devices;
    CSVData parts;

    try {
        employees = CSVData ("./data/employees.csv");
        workOrders = CSVData ("./data/workOrders.csv");
        receiptOrders = CSVData ("./data/receiptOrders.csv");
        users = CSVData ("./data/users.csv");
        devices = CSVData ("./data/devices.csv");
        parts = CSVData ("./data/parts.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        throw std::logic_error ("Neuspjesno kreiranje TXT fajla ");
    }  //------------------
    int foundWorkOrderRow = -1;
    int rowIndex = -1;

    for (rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (workOrders.get_value (rowIndex, 0)) == workOrderId) {
            foundWorkOrderRow = rowIndex;
            break;
        }
    }
    if (foundWorkOrderRow == -1) {
        std::cerr << "Radni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::string receiptOrderId = workOrders.get_value (foundWorkOrderRow, 1);
    std::string comment = workOrders.get_value (foundWorkOrderRow, 3);
    std::string endDate = workOrders.get_value (foundWorkOrderRow, 5);
    std::string technicianId = workOrders.get_value (foundWorkOrderRow, 6);
    std::string usedParts = workOrders.get_value (foundWorkOrderRow, 7);
    std::string servicePrice = workOrders.get_value (foundWorkOrderRow, 8);

    int foundReceiptOrderRow = -1;

    for (rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == std::stoi (receiptOrderId)) {
            foundReceiptOrderRow = rowIndex;
            break;
        }
    }
    if (foundReceiptOrderRow == -1) {
        std::cerr << "Prijemni nalog sa zadatim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::string userID = receiptOrders.get_value (foundReceiptOrderRow, 1);
    std::string deviceIMEI = receiptOrders.get_value (foundReceiptOrderRow, 2);
    std::string description = receiptOrders.get_value (foundReceiptOrderRow, 3);

    int foundUserRow = -1;

    for (rowIndex = 1; rowIndex < users.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (users.get_value (rowIndex, 0)) == std::stoi (userID)) {
            foundUserRow = rowIndex;
            break;
        }
    }
    if (foundUserRow == -1) {
        std::cerr << "Korisnik sa zadatim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::string firstname = users.get_value (foundUserRow, 1);
    std::string lastname = users.get_value (foundUserRow, 2);
    std::string phone = users.get_value (foundUserRow, 4);

    std::map<int, int> dijelovi = stringToParts (usedParts);

    int foundEmployeeRow = -1;

    for (rowIndex = 1; rowIndex < employees.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (employees.get_value (rowIndex, 0) == (technicianId)) {
            if (foundEmployeeRow == -1) {
                foundEmployeeRow = rowIndex;
                break;
            }
        }
    }
    if (foundEmployeeRow == -1) {
        std::cerr << "Tehnicar sa zadatim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    int foundDeviceRow = -1;

    for (rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (devices.get_value (rowIndex, 3)) == std::stoi (deviceIMEI)) {
            foundDeviceRow = rowIndex;
            break;
        }
    }
    if (foundDeviceRow == -1) {
        std::cerr << "Uredjaj sa zadatim IMEI-om nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::string brand = devices.get_value (foundDeviceRow, 1);
    std::string model = devices.get_value (foundDeviceRow, 2);
    std::string state = devices.get_value (foundDeviceRow, 4);

    int foundPartIndex = -1;
    bool partFound = false;
    std::string technicianName = employees.get_value (foundEmployeeRow, 1) + " " + employees.get_value (foundEmployeeRow, 2);

    std::string fileName = "nalog_" + std::to_string (workOrderId);

    std::ofstream file ("./WorkOrders/" + fileName + ".txt");

    if (!file) {
        std::cout << "Nije moguce kreirati fajl!\n";
        return;
    }
    file << "====== PRIJEMNI NALOG ======\n";
    file << "ID: " << workOrderId << "\n";
    file << "ID prijemnog naloga: " << receiptOrderId << "\n";
    file << "Datum: " << formatTimestamp (endDate) << "\n";
    file << "Serviser: " << technicianName << "\n";
    file << "\n";

    file << "Korisnik: " << firstname << " " << lastname << "\n";
    file << "Telefon: " << phone << "\n";
    file << "\n";
    file << "Marka: " << brand << "\n";
    file << "IMEI uredjaja: " << deviceIMEI << "\n";
    file << "Model: " << model << "\n";
    file << "Opis: " << description << "\n";
    file << "Stanje uredjaja: " << state << "\n";
    for (const auto& [partId, qty] : dijelovi) {
        for (int rowIndex = 1; rowIndex < parts.rows (); rowIndex++) {  // Start from 1 to skip header row
            if (std::stoi (parts.get_value (rowIndex, 0)) == partId) {
                std::string partName = parts.get_value (rowIndex, 1);
                file << "  - " << partName << ": " << qty << " kom.\n";
                break;
            }
        }
    }
    file << "\n";
    file << "Cijena rada: " << servicePrice << "\n";
    file.close ();
    std::cout << "Fajl uspjesno kreiran:" << fileName << "\n";
}

double WorkOrderManager::calculateTotalPrice (int workOrderId) {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        throw std::logic_error ("Neuspjesno racunanje ukupne cijene radnog naloga ");
    }  //------------------

    double totalPrice = 0.0;
    for (int rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (workOrders.get_value (rowIndex, 0)) == workOrderId) {
            // Dodavanje cijene dijelova
            for (const auto& [partId, qty] : stringToParts (workOrders.get_value (rowIndex, 7))) {
                totalPrice += partManager.getPartPrice (partId) * qty;
            }

            // Dodavanje cijene rada
            totalPrice += std::stod (workOrders.get_value (rowIndex, 8));
            return totalPrice;
        }
    }
    throw std::logic_error ("Radni nalog sa unesenim ID-em nije pronadjen.");
}

void WorkOrderManager::mainWorkOrdersManager (int id) {
    int choice;
    do {
        std::cout << "\n----- MENADZER RADNIH NALOGA -----" << std::endl;
        std::cout << "1. Kreiraj radni nalog" << std::endl;
        std::cout << "2. Prikazi radne naloge" << std::endl;
        std::cout << "3. Azuriraj radni nalog" << std::endl;
        std::cout << "4. Obrisi radni nalog" << std::endl;
        std::cout << "5. Prikazi gotove radne naloge" << std::endl;
        std::cout << "0. Izlaz iz menadzera radnih naloga" << std::endl;

        std::cout << "Unesite vas izbor: ";
        std::cin >> choice;
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');  // Clear newline character from input buffer

        switch (choice) {
            case 1:
                createWorkOrder (id);
                break;
            case 2:
                listWorkOrders ();
                break;
            case 3:
                updateWorkOrders ();
                break;
            case 4:
                deleteWorkOrder ();
                break;
            case 5:
                listCompletedWorkOrders ();
                break;
            case 0:
                std::cout << "Izlaz iz menadzera radnih naloga." << std::endl;
                break;
            default:
                std::cout << "Pogresan unos. Pokusajte ponovo." << std::endl;
        }
    } while (choice != 0);
}