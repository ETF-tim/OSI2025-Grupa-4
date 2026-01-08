#include "../../include/employee/devicesManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Device.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

// Function for converting string to state enum
State stringToState (const std::string& stateStr) {
    if (stateStr == "NEW")
        return State::NEW;
    else if (stateStr == "USED")
        return State::USED;
    else
        throw std::invalid_argument ("Neispravan string za State");
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
    std::string stanje;
    State myState;
    do {
        std::cout << "Unesite stanje uredjaja: ";
        std::cin >> stanje;
        for (char& c : stanje) {
            c = std::toupper (c);
        }
        myState = stringToState (stanje);
    } while (!Validate::isValidState (myState));
    std::cin.ignore ();  // Clear newline character from input buffer
    //------------------

    // Re-add header and new device to CSV data
    devices.add_row (header, 0);  // Re-add header row
    devices.add_row ({std::to_string (tempId), brand, model, IMEI, stanje}, tempId + 1);

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

    // Print parts list
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

    // Pring parts list
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
            do {
                std::cout << "Unesite novo stanje uredjaja: ";
                std::cin >> newStateStr;
                for (char& c : newStateStr) {
                    c = std::toupper (c);
                }
                myState = stringToState (newStateStr);

            } while (!Validate::isValidState (myState));
            newItem = newStateStr;
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
    bool partFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (devices.get_value (rowIndex, 0)) == deleteId) {
            partFound = true;
            break;
        }
    }
    if (!partFound) {
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