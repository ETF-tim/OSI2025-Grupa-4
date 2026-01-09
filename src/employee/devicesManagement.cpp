#include "../../include/employee/devicesManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Device.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

// Function for converting string to state enum
std::string DeviceManager::stateToString (int stateInt) {
    if (stateInt == 1)
        return "NEW";
    else if (stateInt == 2)
        return "USED";
    else
        return "UNKNOWN";
}

void DeviceManager::createDevice () {
    // Opening CSV file

    CSVData devices;
    try {
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog uredjaja";
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = devices.get_row (0);
    devices.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (devices);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog uredjaja";
        return;
    }  //------------------

    // Input and validation for attributes

    // -> Input and validation for device brand
    std::string brand;
    do {
        std::cout << "Unesite naziv brenda uredjaja: ";
        std::getline (std::cin, brand);
    } while (!Validate::isValidBrand (brand));
    // -> Input and validation for device model

    std::string model;
    do {
        std::cout << "Unesite naziv modela uredjaja: ";
        std::getline (std::cin, model);
    } while (!Validate::isValidModel (model));

    // -> Input and validation for device IMEI
    std::string IMEI;
    do {
        std::cout << "Unesite IMEI uredjaja: ";
        std::getline (std::cin, IMEI);
    } while (!Validate::isValidIMEI (IMEI));

    // -> Input and validation for part price
    std::cout << "Izaberite stanje uredjaja:" << std::endl;
    std::cout << "1. NEW" << std::endl;
    std::cout << "2. USED" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj stanja (1-2): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 2);
    //------------------

    // Re-add header and new device to CSV data
    devices.add_row (header, 0);  // Re-add header row
    devices.add_row ({std::to_string (tempId), brand, model, IMEI, stateToString (attributeChoice)});

    //------------------

    // Writing updated data back to CSV file
    devices.write_data ("./data/devices.csv");
    //------------------
    std::cout << "Uspjesno kreiran novi uredjaj!" << std::endl;
}

void DeviceManager::listDevice () {
    // Opening CSV file
    CSVData devices;
    try {
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste uredjaja";
        return;
    }  //------------------

    // Print device list
    std::cout << "----- LISTA UREDJAJA -----" << std::endl;
    devices.print_csv_data ();
}

void DeviceManager::editDevice () {
    // Opening CSV file
    CSVData devices;
    try {
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno azuriranje uredjaja";
        return;
    }  //------------------

    // Print device list
    listDevice ();

    // Choose device to edit by ID and check if it exists
    // -> Choose device to edit by ID
    std::cout << "Unesite ID uredjaja koji zelite urediti: ";
    int editId;
    std::cin >> editId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool deviceFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (devices.get_value (rowIndex, 0)) == editId) {
            deviceFound = true;
            break;
        }
    }
    if (!deviceFound) {
        std::cerr << "Uredjaj sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Choose attribute to edit by ID
    std::cout << "Koji atribut zelite urediti?" << std::endl;
    std::cout << "1. Brend uredjaja" << std::endl;
    std::cout << "2. Model uredjaja" << std::endl;
    std::cout << "3. IMEI uredjaja" << std::endl;
    std::cout << "4. Stanje uredjaja" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj atributa (1-4): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 4);
    //------------------

    // Input and validation for new attribute value
    std::string newBrand, newModel, newIMEI, newStateStr, newItem;
    State myState;
    switch (attributeChoice) {
        case 1: {  // Brand name
            do {
                std::cout << "Unesite novi naziv dijela: ";
                std::getline (std::cin, newBrand);
            } while (!Validate::isValidBrand (newBrand));
            newItem = newBrand;
            break;
        }
        case 2: {  // Model name

            do {
                std::cout << "Unesite novi model uredjaja: ";
                std::getline (std::cin, newModel);
            } while (!Validate::isValidModel (newModel));
            newItem = newModel;
            break;
        }
        case 3: {  // IMEI number
            do {
                std::cout << "Unesite novi IMEI uredjaja: ";
                std::getline (std::cin, newIMEI);
            } while (!Validate::isValidIMEI (newIMEI));
            newItem = newIMEI;
            break;
        }
        case 4: {  // state
            std::cout << "Izaberite novo stanje uredjaja:" << std::endl;
            std::cout << "1. NEW" << std::endl;
            std::cout << "2. USED" << std::endl;
            int attributeChoice;
            do {
                std::cout << "Unesite broj stanja (1-2): ";
                std::cin >> attributeChoice;
                std::cin.ignore ();  // Clear newline character from input buffer
            } while (attributeChoice < 1 || attributeChoice > 2);
            newItem = stateToString (attributeChoice);
            break;
        }
    }  //------------------

    // Update part attribute in CSV data
    devices.set_value (rowIndex, attributeChoice, newItem);
    //------------------

    // Writing updated data back to CSV file
    devices.write_data ("./data/devices.csv");
    //------------------

    // Print updated devices list
    listDevice ();
    std::cout << "Uspjesno uređen uredjaj!" << std::endl;
}

void DeviceManager::deleteDevice () {
    // Opening CSV file
    CSVData devices;
    try {
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje uredjaja";
        return;
    }  //------------------

    // Print devices list
    listDevice ();
    //------------------

    // Choose device to delete by ID and check if it exists
    // -> Choose device to delete by ID
    std::cout << "Unesite ID uredjaja koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool serviceReportFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (devices.get_value (rowIndex, 0)) == deleteId) {
            serviceReportFound = true;
            break;
        }
    }
    if (!serviceReportFound) {
        std::cerr << "Uredjaj sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete part from CSV data
    devices.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    devices.write_data ("./data/devices.csv");
    // Print updated devices list
    listDevice ();
    std::cout << "Uspjesno obrisan uredjaj!" << std::endl;
}

bool DeviceManager::searchForDevice (int deviceId) {
    // Opening CSV file
    CSVData devices;
    try {
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje uredjaja";
        return false;
    }  //------------------

    // Find device by ID
    for (int rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (devices.get_value (rowIndex, 0)) == deviceId) {
            return true;
        }
    }
    return false;
}

std::string DeviceManager::getDeviceIMEI (int deviceId) {
    // Opening CSV file
    CSVData devices;
    try {
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje uredjaja";
        return "";
    }  //------------------

    // Find device by ID and return IMEI if it exists
    for (int rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (devices.get_value (rowIndex, 0)) == deviceId) {
            return devices.get_value (rowIndex, 3);
        }
    }
    return "";
}