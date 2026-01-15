#include "../../include/employee/serviceReportsManagement.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/WorkOrder.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

std::map<int, int> stringToParts (const std::string s) {
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

int ServiceReportManager::createServiceReport (int WorkOrderID) {
    // Opening CSV file

    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog servisnog izvjestaja";
        return -1;
    }

    // Store and remove header row
    std::vector<std::string> header = serviceReports.get_row (0);
    serviceReports.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (serviceReports);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog uredjaja";
        return -1;
    }  //------------------

    serviceReports.add_row (header, 0);  // Re-add header row
    serviceReports.add_row ({std::to_string (tempId), std::to_string (WorkOrderID)});

    //------------------

    // Writing updated data back to CSV file
    serviceReports.write_data ("./data/serviceReports.csv");
    //------------------
    std::cout << "Uspjesno kreiran novi servisni izvjestaj!" << std::endl;
    return tempId;
}

void ServiceReportManager::listServiceReport () {
    // Opening CSV file
    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste servisnih izvjestaja";
        return;
    }  //------------------

    // Print device list
    std::cout << "----- LISTA SERVISNIH IZVJESTAJA -----" << std::endl;
    serviceReports.print_csv_data ();
}

void ServiceReportManager::deleteServiceReport () {
    // Opening CSV file
    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje servisnog izvjestaja";
        return;
    }  //------------------

    // Print devices list
    listServiceReport ();
    //------------------

    // Choose service report to delete by ID and check if it exists
    // -> Choose service report to delete by ID
    std::cout << "Unesite ID servisnih izvjestaja koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool reportFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < serviceReports.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (serviceReports.get_value (rowIndex, 0)) == deleteId) {
            reportFound = true;
            break;
        }
    }
    if (!reportFound) {
        std::cerr << "Servisni izvjestaj sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete part from CSV data
    serviceReports.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    serviceReports.write_data ("./data/serviceReports.csv");
    // Print updated devices list
    listServiceReport ();
    std::cout << "Uspjesno obrisan servisni izvjestaj!" << std::endl;
}

bool ServiceReportManager::searchForServiceReport (int serviceReportID) {
    // Opening CSV file
    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje servisnog izvjestaja";
        return false;
    }  //------------------

    // Find device by ID
    for (int rowIndex = 1; rowIndex < serviceReports.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (serviceReports.get_value (rowIndex, 0)) == serviceReportID) {
            return true;
        }
    }
    return false;
}
void ServiceReportManager::generateServiceReportTXTFile (int serviceReportID, int billID) {
    CSVData serviceReport;
    CSVData workOrders;
    CSVData receiptOrders;
    CSVData users;
    CSVData devices;
    CSVData parts;
    CSVData bills;

    try {
        serviceReport = CSVData ("./data/serviceReports.csv");
        workOrders = CSVData ("./data/workOrders.csv");
        receiptOrders = CSVData ("./data/receiptOrders.csv");
        users = CSVData ("./data/users.csv");
        devices = CSVData ("./data/devices.csv");
        parts = CSVData ("./data/parts.csv");
        bills = CSVData ("./data/bills.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        throw std::logic_error ("Neuspjesno kreiranje TXT fajla ");
    }  //------------------
    int foundserviceReportRow = -1;
    int rowIndex = -1;

    for (rowIndex = 1; rowIndex < serviceReport.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (serviceReport.get_value (rowIndex, 0)) == serviceReportID) {
            foundserviceReportRow = rowIndex;
            break;
        }
    }
    if (foundserviceReportRow == -1) {
        std::cerr << "Servisni izvjestaj nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::string workOrderId = serviceReport.get_value (foundserviceReportRow, 1);  // FIX: changed from workOrders to serviceReport
    int foundWorkOrderRow = -1;

    for (rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if ((workOrders.get_value (rowIndex, 0)) == workOrderId) {
            foundWorkOrderRow = rowIndex;
            break;
        }
    }
    if (foundWorkOrderRow == -1) {
        std::cerr << "Radni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    int foundBillRow = -1;
    for (rowIndex = 1; rowIndex < bills.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (bills.get_value (rowIndex, 0)) == billID) {
            foundBillRow = rowIndex;
            break;
        }
    }
    if (foundBillRow == -1) {
        std::cerr << "Racun sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::string receiptOrderId = workOrders.get_value (foundWorkOrderRow, 1);
    std::string comment = workOrders.get_value (foundWorkOrderRow, 3);
    std::string usedParts = workOrders.get_value (foundWorkOrderRow, 7);
    std::string servicePrice = bills.get_value (foundBillRow, 3);

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

    int foundDeviceRow = -1;

    for (rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (devices.get_value (rowIndex, 3) == deviceIMEI) {
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
    std::filesystem::create_directory ("./data/ServiceReports");

    std::string fileName = "nalog_" + std::to_string (serviceReportID);

    std::ofstream file ("./data/ServiceReports/" + fileName + ".txt");

    if (!file) {
        std::cout << "Nije moguce kreirati fajl!\n";
        return;
    }
    file << "====== SERVISNI IZVJESTAJ ======\n";

    file << "Korisnik: " << firstname << " " << lastname << "\n";
    file << "Telefon: " << phone << "\n";
    file << "\n";
    file << "Marka: " << brand << "\n";
    file << "IMEI uredjaja: " << deviceIMEI << "\n";
    file << "Model: " << model << "\n";
    file << "Opis: " << description << "\n";
    file << "Komentar servisera: " << comment << "\n";

    file << "Dijelovi korisceni u popravci:" << "\n";
    for (const auto& [partId, qty] : dijelovi) {
        for (int rowIndex = 1; rowIndex < parts.rows (); rowIndex++) {  // Start from 1 to skip header row
            if (std::stoi (parts.get_value (rowIndex, 0)) == partId) {
                std::string partName = parts.get_value (rowIndex, 1);
                file << "  - " << partName << "\n";
                break;
            }
        }
    }
    file << "\n";
    file << "Cijena rada: " << servicePrice << "\n";
    file << "\nPotpis servisa: ____________\n";
    file << "Potpis klijenta: ___________\n";

    file.close ();
    std::cout << "Fajl uspjesno kreiran:" << fileName << "\n";
}

void ServiceReportManager::mainServiceReportsManager () {
    int choice;
    do {
        std::cout << "\n----- MENADZER SERVISNIH IZVJESTAJA -----" << std::endl;
        std::cout << "1. Prikazi servisne izvjestaje" << std::endl;
        std::cout << "2. Obrisi servisni izvjestaj" << std::endl;
        std::cout << "0. Izlaz iz menadzera servisnih izvjestaja" << std::endl;
        std::cout << "Unesite vas izbor: ";
        std::cin >> choice;
        std::cin.ignore ();
        switch (choice) {
            case 1:
                listServiceReport ();
                break;
            case 2:
                deleteServiceReport ();
                break;
            case 0:
                std::cout << "Izlaz iz menadzera servisnih izvjestaja." << std::endl;
                break;
            default:
                std::cout << "Nepostojeca opcija. Pokusajte ponovo." << std::endl;
                break;
        }
    } while (choice != 0);
}
