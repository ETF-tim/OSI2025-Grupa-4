#include "../../include/employee/usersManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Part.hpp"
#include "../../include/common/util/GeneratePIN.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

void createUser () {
    // Opening CSV file
    CSVData users;
    try {
        users = CSVData ("./data/users.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog korisnika";
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = users.get_row (0);
    users.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (users);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog korisnika";
        return;
    }  //------------------

    // Input and validation for attributes

    // -> Input and validation for user first name
    std::string tempFirstName;
    do {
        std::cout << "Unesite ime korisnika: ";
        std::getline (std::cin, tempFirstName);
    } while (!Validate::isValidFirstName (tempFirstName));

    // -> Input and validation for user last name
    std::string tempLastName;
    do {
        std::cout << "Unesite prezime korisnika: ";
        std::getline (std::cin, tempLastName);
    } while (!Validate::isValidLastName (tempLastName));

    // -> Input and validation for user email
    std::string tempEmail;
    do {
        std::cout << "Unesite email korisnika: ";
        std::getline (std::cin, tempEmail);
    } while (!Validate::isValidEmail (tempEmail));

    // -> Input and validation for user phone
    std::string tempPhone;
    do {
        std::cout << "Unesite broj telefona korisnika: ";
        std::getline (std::cin, tempPhone);
    } while (!Validate::isValidPhone (tempPhone));

    // -> Generate PIN for user account
    int tempPin = generatePIN ();
    // -------------------

    // Re-add header and new part to CSV data
    users.add_row (header, 0);  // Re-add header row
    users.add_row ({std::to_string (tempId), tempFirstName, tempLastName, tempEmail, tempPhone, std::to_string (tempPin)});
    //------------------

    // Writing updated data back to CSV file
    users.write_data ("./data/users.csv");
    //------------------

    std::cout << "Uspjesno kreiran novi korisnik!" << std::endl;
}

void listUsers () {
    // Opening CSV file
    CSVData users;
    try {
        users = CSVData ("./data/users.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste korisnika";
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = users.get_row (0);
    users.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by part ID in ascending order
    users.sort_by_col (0, CSVData::ASC);

    // Re-add header row
    users.add_row (header, 0);  // Re-add header row
    //------------------

    // Print parts list
    std::cout << "----- LISTA KORISNIKA -----" << std::endl;
    users.print_csv_data ();
}

void editUser () {
    // Opening CSV file
    CSVData users;
    try {
        users = CSVData ("./data/users.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno azuriranje podataka korisnika";
        return;
    }  //------------------

    // Pring users list
    listUsers ();

    // Choose user to edit by ID and check if it exists
    // -> Choose user to edit by ID
    std::cout << "Unesite ID korisnika koji zelite urediti: ";
    int editId;
    std::cin >> editId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool userFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < users.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (users.get_value (rowIndex, 0)) == editId) {
            userFound = true;
            break;
        }
    }
    if (!userFound) {
        std::cerr << "Korisnik sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Choose attribute to edit by ID
    std::cout << "Koji atribut zelite urediti?" << std::endl;
    std::cout << "1. Ime korisnika" << std::endl;
    std::cout << "2. Prezime korisnika" << std::endl;
    std::cout << "3. Email korisnika" << std::endl;
    std::cout << "4. Broj telefona korisnika" << std::endl;
    std::cout << "5. PIN korisnika" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj atributa (1-5): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 5);
    //------------------

    // Input and validation for new attribute value
    std::string newValue;
    switch (attributeChoice) {
        case 1: {  // User first name
            do {
                std::cout << "Unesite novo ime korisnika: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidFirstName (newValue));
            break;
        }
        case 2: {  // User last name
            do {
                std::cout << "Unesite novo prezime korisnika: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidLastName (newValue));
            break;
        }
        case 3: {  // User email
            do {
                std::cout << "Unesite novu email adresu korisnika: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidEmail (newValue));
            break;
        }
        case 4: {  // User phone number
            do {
                std::cout << "Unesite novi broj telefona korisnika: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidPhone (newValue));
            break;
        }
        case 5: {  // User PIN
            std::cout << "Generisanje novog PIN-a za korisnika..." << std::endl;
            newValue = std::to_string (generatePIN ());
            break;
        }
    }  //------------------

    // Update part attribute in CSV data
    users.set_value (rowIndex, attributeChoice, newValue);
    //------------------

    // Writing updated data back to CSV file
    users.write_data ("./data/users.csv");
    //------------------

    // Print updated parts list
    listUsers ();
    std::cout << "Uspjesno azuriran podatak korisnika!" << std::endl;
}

void deleteUser () {
    // Opening CSV file
    CSVData users;
    try {
        users = CSVData ("./data/users.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje korisnika";
        return;
    }  //------------------

    // Pring parts list
    listUsers ();
    //------------------

    // Choose user to delete by ID and check if it exists
    // -> Choose user to delete by ID
    std::cout << "Unesite ID korisnika koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool userFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < users.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (users.get_value (rowIndex, 0)) == deleteId) {
            userFound = true;
            break;
        }
    }
    if (!userFound) {
        std::cerr << "Korisnik sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete user from CSV data
    users.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    users.write_data ("./data/users.csv");
    //------------------

    // Print updated users list
    listUsers ();
    std::cout << "Uspjesno obrisan korisnik!" << std::endl;
}
