#include "../../include/employee/workOrdersManagement.hpp"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../../include/common/models/WorkOrder.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/partsManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"
#include "../../include/employee/workOrdersManagement.hpp"

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
    if (!receiptFound || !userFound) {
        std::cerr << "Greska pri pronalazenju podataka o klijentu." << std::endl;
        return;
    }

    IMEIUredjaja = receiptOrders.get_value (foundReceiptOrderIndex, 2);
    imeKlijenta = users.get_value (foundUserIndex, 1);
    prezimeKlijenta = users.get_value (foundUserIndex, 2);

    std::cout << "Ime klijenta: " << imeKlijenta << std::endl;
    std::cout << "Prezime klijenta: " << prezimeKlijenta << std::endl;
    std::cout << "IMEI uredjaja klijenta: " << IMEIUredjaja << std::endl;

    std::cout << "Da li su ovi podaci tacni? (d/n): ";
    char confirm;
    do {
        std::cin >> confirm;
        std::cin.ignore ();  // Clear newline character from input buffer}
    } while (confirm != 'd' && confirm != 'D' && confirm != 'n' && confirm != 'N');
    if (confirm == 'n' || confirm == 'N') {
        std::cout << "Kreiranje radnog naloga prekinuto." << std::endl;
        return;
    }

    std::cout << "Promjena statusa prijemnog naloga sa slobodan na zauzet" << std::endl;
    receiptOrderManager.changeReceiptOrderStatus (temptReceiptOrderID, 0);  // change receipt order status from free to not free

    // Set initial status to IN_DIAGNOSTICS when starting work on device
    WorkOrderStatus currentStatus = WorkOrderStatus::IN_DIAGNOSTICS;
    std::cout << "\n Radni nalog je kreiran. Status: IN_DIAGNOSTICS" << std::endl;
    std::cout << "Tehnicar pocinje sa dijagnostikom uredjaja...\n" << std::endl;

    std::time_t startDate = std::time (nullptr);  // Current date/time
    std::time_t endDate = 0;
    double servicePrice = 0;  // Will be updated when work is COMPLETED

    std::string comment;
    std::cout << "Da li zelite uneti komentar: (d/n)" << std::endl;
    do {
        std::cout << "Unesite odgovor: ";
        std::cin >> confirm;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (confirm != 'd' && confirm != 'D' && confirm != 'n' && confirm != 'N');

    if (confirm == 'd' || confirm == 'D') {
        std::cin.ignore ();
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
    std::cin.ignore ();  // Clear newline character from input buffer
    // ----------------

    for (int rowIndex = 0; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
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

    std::cout << "Azuriranje dijelova u popravci uredjaja." << std::endl;
    std::cout << "Ako nema dodavanja novih dijelova, u sljedecoj poruci unesite n/N" << std::endl;

    while (true) {
        std::cout << "Dodaj dio u popravku? (d/n):";
        char choice;
        do {
            std::cin >> choice;
            std::cin.ignore ();  // Clear newline from buffer
        } while (choice != 'd' && choice != 'D' && choice != 'n' && choice != 'N');

        if (choice == 'n' || choice == 'N') break;  // If input is 'n', exit from the loop

        int partId, quantity;

        std::cout << "Unesite id dijela koji zelite dodati: ";
        std::cin >> partId;
        std::cout << "Unesite kolicinu dijela koji zelite uzeti sa skladista: ";
        std::cin >> quantity;
        std::cin.ignore ();  // Clear newline from buffer

        // Check 1: does part exist
        if (!partManager.searchForPart (partId)) {
            std::cout << "Dio sa unesenim ID-em nije pronadjen." << std::endl;
            continue;
        }

        // Check 2: is part available with this quantity
        if (!partManager.isPartAvailable (partId, quantity)) {
            std::cout << "Nema dovoljno delova u skladistu." << std::endl;
            std::cout << "Mozete promeniti status u WAITING_FOR_PARTS nakon zavrsetka unosa." << std::endl;
            continue;
        }

        partManager.decreasePartStock (partId, quantity);

        usedParts[partId] += quantity;
        std::cout << "Dio je dodan u popravku!" << std::endl;
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
        std::cin.ignore ();  // Clear newline character from input buffer
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
    char response;
    std::cout << "Da li zelite uneti komentar: (d/n)" << std::endl;
    do {
        std::cin >> response;
        std::cin.ignore ();  // Clear newline from buffer
    } while (response != 'd' && response != 'D' && response != 'n' && response != 'N');

    if (response == 'd' || response == 'D') {
        std::cin.ignore ();
        std::cout << "Unesite komentar:" << std::endl;
        std::getline (std::cin, comment);
    }

    std::cout << "Da li zelite promijeniti cijenu popravke uredjaja: (d/n)" << std::endl;
    do {
        std::cin >> response;
        std::cin.ignore ();  // Clear newline from buffer
    } while (response != 'd' && response != 'D' && response != 'n' && response != 'N');

    if (response == 'd' || response == 'D') {
        std::cin.ignore ();
        std::cout << "Unesite cijenu:" << std::endl;
        std::cin >> servicePrice;
    }

    std::cout << "Da li je radni nalog zaista zavrsen: (d/n)" << std::endl;
    do {
        std::cout << "Unesite odgovor: ";
        std::cin >> response;
        std::cin.ignore ();
        // Clear newline character from input buffer
    } while (response != 'd' && response != 'D' && response != 'n' && response != 'N');

    if (response == 'd' || response == 'D') {
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

    // Filtering free work orders (delete where taken status is true)
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
