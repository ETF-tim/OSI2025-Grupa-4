#include "../../include/employee/partsManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Part.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

void createPart () {
    // Opening CSV file
    CSVData parts;
    try {
        parts = CSVData ("./data/parts.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog dijela";
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = parts.get_row (0);
    parts.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (parts);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog dijela";
        return;
    }  //------------------

    // Input and validation for attributes

    // -> Input and validation for part name
    std::string tempName;
    do {
        std::cout << "Unesite naziv dijela: ";
        std::getline (std::cin, tempName);
    } while (!Validate::isValidPartName (tempName));

    // -> Input and validation for part amount
    int tempAmount;
    do {
        std::cout << "Unesite kolicinu dijela: ";
        std::cin >> tempAmount;
    } while (!Validate::isValidNonNegative (tempAmount));
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Input and validation for part critical amount
    int tempCriticalAmount;
    do {
        std::cout << "Unesite kriticnu kolicinu dijela: ";
        std::cin >> tempCriticalAmount;
    } while (!Validate::isValidNonNegative (tempCriticalAmount));
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Input and validation for part price
    double tempPrice;
    do {
        std::cout << "Unesite cijenu dijela: ";
        std::cin >> tempPrice;
    } while (!Validate::isValidPrice (tempPrice));
    std::cin.ignore ();  // Clear newline character from input buffer
    //------------------

    // Creating new Part object
    try {
        Part newPart (tempId, tempName, tempAmount, tempCriticalAmount, tempPrice);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog dijela";
        return;
    }  //------------------

    // Re-add header and new part to CSV data
    parts.add_row (header, 0);  // Re-add header row
    parts.add_row ({std::to_string (tempId), tempName, std::to_string (tempAmount), std::to_string (tempCriticalAmount),
                    std::to_string (tempPrice)});
    //------------------

    // Writing updated data back to CSV file
    parts.write_data ("./data/parts.csv");
    //------------------
    std::cout << "Uspjesno kreiran novi dio!" << std::endl;
}

void listParts () {
    // Opening CSV file
    CSVData parts;
    try {
        parts = CSVData ("./data/parts.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog dijela";
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = parts.get_row (0);
    parts.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by part ID in ascending order
    parts.sort_by_col (0, CSVData::ASC);

    // Re-add header row
    parts.add_row (header, 0);  // Re-add header row
    //------------------

    // Print parts list
    std::cout << "----- LISTA DIJELOVA -----" << std::endl;
    parts.print_csv_data ();
}

void editPart () {
    // Opening CSV file
    CSVData parts;
    try {
        parts = CSVData ("./data/parts.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog dijela";
        return;
    }  //------------------

    // Pring parts list
    listParts ();

    // Choose part to edit by ID and check if it exists
    // -> Choose part to edit by ID
    std::cout << "Unesite ID dijela koji zelite urediti: ";
    int editId;
    std::cin >> editId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool partFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < parts.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (parts.get_value (rowIndex, 0)) == editId) {
            partFound = true;
            break;
        }
    }
    if (!partFound) {
        std::cerr << "Dio sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Choose attribute to edit by ID
    std::cout << "Koji atribut zelite urediti?" << std::endl;
    std::cout << "1. Naziv dijela" << std::endl;
    std::cout << "2. Kolicina dijela" << std::endl;
    std::cout << "3. Kriticna kolicina dijela" << std::endl;
    std::cout << "4. Cijena dijela" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj atributa (1-4): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 4);
    //------------------

    // Input and validation for new attribute value
    std::string newValue;
    switch (attributeChoice) {
        case 1: {  // Part name
            do {
                std::cout << "Unesite novi naziv dijela: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidPartName (newValue));
            break;
        }
        case 2: {  // Part amount
            int tempAmount;
            do {
                std::cout << "Unesite novu kolicinu dijela: ";
                std::cin >> tempAmount;
                std::cin.ignore ();  // Clear newline character from input buffer
            } while (!Validate::isValidNonNegative (tempAmount));
            newValue = std::to_string (tempAmount);
            break;
        }
        case 3: {  // Part critical amount
            int tempCriticalAmount;
            do {
                std::cout << "Unesite novu kriticnu kolicinu dijela: ";
                std::cin >> tempCriticalAmount;
                std::cin.ignore ();  // Clear newline character from input buffer
            } while (!Validate::isValidNonNegative (tempCriticalAmount));
            newValue = std::to_string (tempCriticalAmount);
            break;
        }
        case 4: {  // Part price
            double tempPrice;
            do {
                std::cout << "Unesite novu cijenu dijela: ";
                std::cin >> tempPrice;
                std::cin.ignore ();  // Clear newline character from input buffer
            } while (!Validate::isValidPrice (tempPrice));
            newValue = std::to_string (tempPrice);
            break;
        }
    }  //------------------

    // Update part attribute in CSV data
    parts.set_value (rowIndex, attributeChoice, newValue);
    //------------------

    // Writing updated data back to CSV file
    parts.write_data ("./data/parts.csv");
    //------------------

    // Print updated parts list
    listParts ();
    std::cout << "Uspjesno uređen dio!" << std::endl;
}

void deletePart () {
    // Opening CSV file
    CSVData parts;
    try {
        parts = CSVData ("./data/parts.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog dijela";
        return;
    }  //------------------

    // Pring parts list
    listParts ();
    //------------------

    // Choose part to delete by ID and check if it exists
    // -> Choose part to delete by ID
    std::cout << "Unesite ID dijela koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool partFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < parts.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (parts.get_value (rowIndex, 0)) == deleteId) {
            partFound = true;
            break;
        }
    }
    if (!partFound) {
        std::cerr << "Dio sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete part from CSV data
    parts.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    parts.write_data ("./data/parts.csv");

    // Print updated parts list
    listParts ();
    std::cout << "Uspjesno obrisan dio!" << std::endl;
}