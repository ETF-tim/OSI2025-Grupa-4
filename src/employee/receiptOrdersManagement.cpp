#include "../../include/employee/receiptOrdersManagement.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../include/common/models/Receipt_order.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/devicesManagement.hpp"
#include "../../include/employee/usersManagement.hpp"

ReceiptOrderManager::ReceiptOrderManager (UserManager& userManager, DeviceManager& deviceManager)
    : userManager (userManager), deviceManager (deviceManager) {}

void ReceiptOrderManager::createReceiptOrder () {
    std::string confirm;
    do {
        std::cout << "Da li ste sigurni da zelite kreirati prijemni nalog? (d/n): ";
        std::getline (std::cin, confirm);
    } while (confirm != "d" && confirm != "D" && confirm != "n" && confirm != "N");

    if (confirm == "n" || confirm == "N") {
        std::cout << "Kreiranje prijemnog naloga otkazano." << std::endl;
        return;
    }

    // Opening CSV file
    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog prijemnog naloga";
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = receiptOrders.get_row (0);
    receiptOrders.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (receiptOrders);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog prijemnog naloga";
        return;
    }  //------------------

    // Print users list+
    userManager.listUsers ();
    //------------------

    // Choose user by ID to assign with receipt order
    std::string tempUserIdString;
    int tempUserId;
    std::cout << "Unesite ID korisnika za kojeg kreirate prijemni nalog: ";
    std::getline (std::cin, tempUserIdString);
    if (!Validate::isValidInteger (tempUserIdString) && (tempUserId = std::stoi (tempUserIdString)) < 0) {
        std::cerr << "Pogresan unos ID-a korisnika." << std::endl;
        return;
    }  // ----------------

    // Check if user exists
    if (!userManager.searchForUser (tempUserId)) {
        std::cerr << "Korisnik sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Print devices list
    deviceManager.listDevice ();
    //------------------

    // Choose device by ID to assign with receipt order
    std::string tempDeviceIdString;
    int tempDeviceId;
    std::cout << "Unesite ID uredjaja za kojeg kreirate prijemni nalog: ";
    std::getline (std::cin, tempDeviceIdString);
    if (!Validate::isValidInteger (tempDeviceIdString) && (tempDeviceId = std::stoi (tempDeviceIdString)) < 0) {
        std::cerr << "Pogresan unos ID-a uredjaja." << std::endl;
        return;
    }  // ----------------

    // Check if device exists
    if (!deviceManager.searchForDevice (tempDeviceId)) {
        std::cerr << "Uredjaj sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Find Device IMEI
    std::string tempDeviceIMEI = deviceManager.getDeviceIMEI (tempDeviceId);
    //------------------

    // Input and validation for receipt order description
    std::string tempDescription;
    do {
        std::cout << "Unesite opis: ";
        std::getline (std::cin, tempDescription);
    } while (!Validate::isValidDescription (tempDescription));

    // Input and validation for device price assessment
    std::string tempPriceAssessmentString;
    double tempPriceAssessment;
    do {
        std::cout << "Unesite procjenu cijene uredjaja: ";
        std::getline (std::cin, tempPriceAssessmentString);
    } while (!Validate::isValidDouble (tempPriceAssessmentString) ||
             !Validate::isValidPrice (tempPriceAssessment = std::stod (tempPriceAssessmentString)));

    // Re-add header and new part to CSV data
    receiptOrders.add_row (header, 0);  // Re-add header row
    receiptOrders.add_row ({std::to_string (tempId), std::to_string (tempUserId), tempDeviceIMEI, tempDescription,
                            std::to_string (tempPriceAssessment), std::to_string (true)});
    //------------------

    // Writing updated data back to CSV file
    receiptOrders.write_data ("./data/receiptOrders.csv");
    //------------------
    std::cout << "Uspjesno kreiran novi prijemni nalog u bazi!" << std::endl;

    generateReceiptOrderTXTFile (tempId);
}

void ReceiptOrderManager::listReceiptOrders () {
    // Opening CSV file
    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste prijemnih naloga";
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = receiptOrders.get_row (0);
    receiptOrders.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by receiptOrder ID in ascending order
    receiptOrders.sort_by_col (0, CSVData::ASC);

    // Re-add header row
    receiptOrders.add_row (header, 0);  // Re-add header row
    //------------------

    // Print receipt orders list
    std::cout << "----- LISTA PRIJEMNIH NALOGA -----" << std::endl;
    receiptOrders.print_csv_data ();
}

void ReceiptOrderManager::deleteReceiptOrder () {
    // Opening CSV file
    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje prijemnog naloga";
        return;
    }  //------------------

    // Print receipt orders list
    listReceiptOrders ();
    //------------------

    // Choose receipt order to delete by ID and check if it exists
    // -> Choose receipt order to delete by ID
    std::string deleteIdString;
    int deleteId;
    std::cout << "Unesite ID prijemnog naloga koji zelite obrisati: ";
    std::getline (std::cin, deleteIdString);

    if (!Validate::isValidInteger (deleteIdString)) {
        std::cerr << "Pogresan unos ID-a prijemnog naloga." << std::endl;
        return;
    } else {
        deleteId = std::stoi (deleteIdString);
    }

    // -> Check if it exists
    bool receiptOrderFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == deleteId) {
            receiptOrderFound = true;
            break;
        }
    }
    if (!receiptOrderFound) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete receipt order from CSV data
    receiptOrders.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    receiptOrders.write_data ("./data/receiptOrders.csv");

    // Print updated receipt orders list
    listReceiptOrders ();
    std::cout << "Uspjesno obrisan prijemni nalog!" << std::endl;
}

bool ReceiptOrderManager::searchForReceiptOrder (int receiptOrderId) {
    // Opening CSV file
    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje prijemnog naloga";
        return false;
    }  //------------------

    // Check if part exists
    for (int rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == receiptOrderId) {
            return true;
        }
    }
    return false;
}

void ReceiptOrderManager::listFreeReceiptOrders () {
    // Opening CSV file
    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste slobodnih prijemnih naloga";
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = receiptOrders.get_row (0);
    receiptOrders.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by receiptOrder ID in ascending order
    receiptOrders.sort_by_col (0, CSVData::ASC);

    // Re-add header row
    receiptOrders.add_row (header, 0);  // Re-add header row
    //------------------

    // Filtering free receipt orders (delete where taken status is true)
    for (int rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (receiptOrders.get_value (rowIndex, 5) == "0") {
            receiptOrders.delete_row (rowIndex);
            rowIndex--;  // Adjust index after deletion
        }
    }
    //------------------

    // Print free receipt orders list
    std::cout << "----- LISTA SLOBODNIH PRIJEMNIH NALOGA -----" << std::endl;
    receiptOrders.print_csv_data ();
}

bool ReceiptOrderManager::isReceiptOrderFree (int receiptOrderId) {
    // Opening CSV file
    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno provjeravanje statusa prijemnog naloga";
        return false;
    }  //------------------

    for (int rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == receiptOrderId) {
            return receiptOrders.get_value (rowIndex, 5) == "1";
        }
    }
    return false;
}

void ReceiptOrderManager::changeReceiptOrderStatus (int receiptOrderId, bool newStatus) {
    // Opening CSV file
    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        throw std::logic_error ("Neuspjesno mijenjanje statusa prijemnog naloga");
    }  //------------------

    for (int rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == receiptOrderId) {
            receiptOrders.set_value (rowIndex, 5, std::to_string (newStatus));
            break;
        }
    }

    // Writing updated data back to CSV file
    receiptOrders.write_data ("./data/receiptOrders.csv");
}
void ReceiptOrderManager::generateReceiptOrderTXTFile (int receiptOrderId) {
    CSVData receiptOrders;
    CSVData users;
    CSVData devices;

    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
        users = CSVData ("./data/users.csv");
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        throw std::logic_error ("Neuspjesno kreiranje TXT fajla ");
    }  //------------------

    int foundReceiptOrderRow = -1;
    int rowIndex = -1;

    for (rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == receiptOrderId) {
            foundReceiptOrderRow = rowIndex;
            break;
        }
    }
    if (foundReceiptOrderRow == -1) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::string userID = receiptOrders.get_value (foundReceiptOrderRow, 1);
    std::string deviceIMEI = receiptOrders.get_value (foundReceiptOrderRow, 2);
    std::string description = receiptOrders.get_value (foundReceiptOrderRow, 3);
    std::string priceAssessment = receiptOrders.get_value (foundReceiptOrderRow, 4);

    int foundUserRow = -1;

    for (rowIndex = 1; rowIndex < users.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (users.get_value (rowIndex, 0) == userID) {
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
    std::string email = users.get_value (foundUserRow, 3);
    std::string phone = users.get_value (foundUserRow, 4);
    std::string pin = users.get_value (foundUserRow, 5);

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
    std::string state = devices.get_value (foundDeviceRow, 4);

    std::string fileName = "nalog_" + std::to_string (receiptOrderId);
    std::filesystem::create_directory ("./data/ReceiptOrders");

    std::ofstream file ("./data/ReceiptOrders/" + fileName + ".txt");

    if (!file) {
        std::cout << "Nije moguce kreirati fajl!\n";
        return;
    }

    file << "====== PRIJEMNI NALOG ======\n";
    file << "ID: " << receiptOrderId << "\n";
    file << "Korisnik: " << firstname << " " << lastname << "\n";
    file << "Email: " << email << "\n";
    file << "Telefon: " << phone << "\n";
    file << "IMEI uredjaja: " << deviceIMEI << "\n";
    file << "Marka: " << brand << "\n";
    file << "Model: " << model << "\n";
    file << "Opis: " << description << "\n";
    file << "Stanje uredjaja: " << state << "\n";
    file << "Procjena cijene: " << priceAssessment << "\n";
    file.close ();
    std::cout << "Fajl uspjesno kreiran:" << fileName << "\n";
}

void ReceiptOrderManager::mainReceiptOrdersManagement () {
    int choice;
    do {
#ifdef _WIN32
        system ("cls");  // Windows
#else
        system ("clear");  // Linux / macOS
#endif

        std::cout << "\n----- MENADZER PRIJEMNIH NALOGA -----" << std::endl;
        std::cout << "1. Kreiraj prijemni nalog" << std::endl;
        std::cout << "2. Prikazi prijemne naloge" << std::endl;
        std::cout << "3. Obrisi prijemni nalog" << std::endl;
        std::cout << "0. Izlaz iz menadzera prijemnih naloga" << std::endl;

        std::string choiceString;
        do {
            std::cout << "Izaberite opciju (0-3): ";
            std::getline (std::cin, choiceString);
        } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 0 || choice > 3);

        switch (choice) {
            case 1:
                createReceiptOrder ();
                std::cout << "Pritisnite bilo koje dugme za nastavak..." << std::endl;
                std::getline (std::cin, choiceString);
                break;
            case 2:
                listReceiptOrders ();
                std::cout << "Pritisnite bilo koje dugme za nastavak..." << std::endl;
                std::getline (std::cin, choiceString);
                break;
            case 3:
                deleteReceiptOrder ();
                std::cout << "Pritisnite bilo koje dugme za nastavak..." << std::endl;
                std::getline (std::cin, choiceString);
                break;
            case 0:
                std::cout << "Izlaz iz menadzera prijemnih naloga." << std::endl;
                std::cout << "Pritisnite bilo koje dugme za nastavak..." << std::endl;
                std::getline (std::cin, choiceString);
                break;
        }
    } while (choice != 0);
}
