#include "../../include/employee/employeesManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Worker.hpp"
#include "../../include/common/util/GeneratePIN.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

// Function for converting string to state enum
std::string EmployeeManager::roleToString (int roleInt) {
    if (roleInt == 1)
        return "RECEPTIONIST";
    else if (roleInt == 2)
        return "TECHNICIAN";
    else if (roleInt == 3)
        return "SERVICE_OWNER";
    else
        return "UNKNOWN";
}

void EmployeeManager::createEmployee () {
    // Opening CSV file
    CSVData employees;
    try {
        employees = CSVData ("./data/employees.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog zaposlenog" << std::endl;
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = employees.get_row (0);
    employees.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (employees);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog zaposlenog" << std::endl;
        return;
    }  //------------------

    // Input and validation for attributes

    // -> Input and validation for employee first name
    std::string tempFirstName;
    do {
        std::cout << "Unesite ime zaposlenog: ";
        std::getline (std::cin, tempFirstName);
    } while (!Validate::isValidFirstName (tempFirstName));

    // -> Input and validation for employee last name
    std::string tempLastName;
    do {
        std::cout << "Unesite prezime zaposlenog: ";
        std::getline (std::cin, tempLastName);
    } while (!Validate::isValidLastName (tempLastName));

    // -> Input and validation for employee email
    std::string tempEmail;
    do {
        std::cout << "Unesite email zaposlenog: ";
        std::getline (std::cin, tempEmail);
    } while (!Validate::isValidEmail (tempEmail));

    // -> Input and validation for employee phone
    std::string tempPhone;
    do {
        std::cout << "Unesite broj telefona zaposlenog: ";
        std::getline (std::cin, tempPhone);
    } while (!Validate::isValidPhone (tempPhone));

    // -> Generate PIN for employee account
    int tempPin = generatePIN ();
    // -------------------

    // -> Input and validation for employee role
    std::cout << "Izaberite ulogu zaposlenog:" << std::endl;
    std::cout << "1. RECEPCIONER" << std::endl;
    std::cout << "2. TEHNICAR" << std::endl;
    std::cout << "3. VLASNIK SERVISA" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj uloge (1-3): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 3);
    //------------------

    // Re-add header and new employee to CSV data
    employees.add_row (header, 0);  // Re-add header row
    employees.add_row ({std::to_string (tempId), tempFirstName, tempLastName, tempEmail, tempPhone, std::to_string (tempPin),
                        roleToString (attributeChoice)});
    //------------------

    // Writing updated data back to CSV file
    employees.write_data ("./data/employees.csv");
    //------------------

    std::cout << "Uspjesno kreiran novi zaposleni!" << std::endl;
}

void EmployeeManager::listEmployees () {
    // Opening CSV file
    CSVData employees;
    try {
        employees = CSVData ("./data/employees.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste zaposlenih" << std::endl;
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = employees.get_row (0);
    employees.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by employee ID in ascending order
    employees.sort_by_col (0, CSVData::ASC);

    // Re-add header row
    employees.add_row (header, 0);  // Re-add header row
    //------------------

    // Print employees list
    std::cout << "----- LISTA ZAPOSLENIH -----" << std::endl;
    employees.print_csv_data ();
}

void EmployeeManager::editEmployee () {
    // Opening CSV file
    CSVData employees;
    try {
        employees = CSVData ("./data/employees.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno azuriranje podataka zaposlenih" << std::endl;
        return;
    }  //------------------

    // Print employees list
    listEmployees ();

    // Choose employee to edit by ID and check if it exists
    // -> Choose employee to edit by ID
    std::cout << "Unesite ID zaposlenog koji zelite urediti: ";
    int editId;
    std::cin >> editId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool employeeFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < employees.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (employees.get_value (rowIndex, 0)) == editId) {
            employeeFound = true;
            break;
        }
    }
    if (!employeeFound) {
        std::cerr << "Zaposleni sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Choose attribute to edit by ID
    std::cout << "Koji atribut zelite urediti?" << std::endl;
    std::cout << "1. Ime zaposlenog" << std::endl;
    std::cout << "2. Prezime zaposlenog" << std::endl;
    std::cout << "3. Email zaposlenog" << std::endl;
    std::cout << "4. Broj telefona zaposlenog" << std::endl;
    std::cout << "5. PIN zaposlenog" << std::endl;
    std::cout << "6. Uloga zaposlenog" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj atributa (1-6): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 6);
    //------------------

    // Input and validation for new attribute value
    std::string newValue;
    switch (attributeChoice) {
        case 1: {  // Employee first name
            do {
                std::cout << "Unesite novo ime zaposlenog: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidFirstName (newValue));
            break;
        }
        case 2: {  // Employee last name
            do {
                std::cout << "Unesite novo prezime zaposlenog: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidLastName (newValue));
            break;
        }
        case 3: {  // Employee email
            do {
                std::cout << "Unesite novu email adresu zaposlenog: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidEmail (newValue));
            break;
        }
        case 4: {  // Employee phone number
            do {
                std::cout << "Unesite novi broj telefona zaposlenog: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidPhone (newValue));
            break;
        }
        case 5: {  // Employee PIN
            std::cout << "Generisanje novog PIN-a za zaposlenog..." << std::endl;
            newValue = std::to_string (generatePIN ());
            break;
        }
        case 6: {  // Employee role
            std::cout << "Izaberite novu ulogu zaposlenog:" << std::endl;
            std::cout << "1. RECEPCIONER" << std::endl;
            std::cout << "2. TEHNICAR" << std::endl;
            std::cout << "3. VLASNIK SERVISA" << std::endl;
            int roleChoice;
            do {
                std::cout << "Unesite broj uloge (1-3): ";
                std::cin >> roleChoice;
                std::cin.ignore ();  // Clear newline character from input buffer
            } while (roleChoice < 1 || roleChoice > 3);
            newValue = roleToString (roleChoice);
            break;
        }
    }  //------------------

    // Update employee attribute in CSV data
    employees.set_value (rowIndex, attributeChoice, newValue);
    //------------------

    // Writing updated data back to CSV file
    employees.write_data ("./data/employees.csv");
    //------------------

    // Print updated employees list
    listEmployees ();
    std::cout << "Uspjesno azuriran podatak zaposlenog!" << std::endl;
}

void EmployeeManager::deleteEmployee () {
    // Opening CSV file
    CSVData employees;
    try {
        employees = CSVData ("./data/employees.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje zaposlenog" << std::endl;
        return;
    }  //------------------

    // Printing employees list
    listEmployees ();
    //------------------

    // Choose employee to delete by ID and check if it exists
    // -> Choose employee to delete by ID
    std::cout << "Unesite ID zaposlenog koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool employeeFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < employees.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (employees.get_value (rowIndex, 0)) == deleteId) {
            employeeFound = true;
            break;
        }
    }
    if (!employeeFound) {
        std::cerr << "Zaposleni sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete employee from CSV data
    employees.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    employees.write_data ("./data/employees.csv");
    //------------------

    // Print updated employees list
    listEmployees ();
    std::cout << "Uspjesno obrisan zaposleni!" << std::endl;
}

bool EmployeeManager::searchForEmployee (int employeeId) {
    // Opening CSV file
    CSVData employees;
    try {
        employees = CSVData ("./data/employees.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje zaposlenih" << std::endl;
        return false;
    }  //------------------

    // Check if employee exists
    for (int rowIndex = 1; rowIndex < employees.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (employees.get_value (rowIndex, 0)) == employeeId) {
            return true;
        }
    }
    return false;
}

void EmployeeManager::mainEmployeesManagement() {

    
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n========================================\n";
        std::cout << "     SISTEM ZA UPRAVLJANJE ZAPOSLENIMA    \n";
        std::cout << "========================================\n";
        std::cout << "1. Pregled svih zaposlenih\n";
        std::cout << "2. Kreiranje novog zaposlenog\n";
        std::cout << "3. Azuriranje podataka o zaposlenom\n";
        std::cout << "4. Brisanje zaposlenog\n";
        std::cout << "0. Za povratak na izborni meni\n";
        std::cout << "----------------------------------------\n";
        std::cout << "Unesite izbor: ";
        std::cin >> choice;
        
        switch (choice) {
            case 1:
                listEmployees();
                break;
                
            case 2:
                createEmployee();
                break;
                
            case 3:
                editEmployee();
                break;
                
                break;
                
            case 4:
                deleteEmployee();
                break;
                
            case 0:
                break;
                
            default:
                std::cout << "Pogresan izbor. Pokusajte ponovo.\n";
                break;
        }
    }
}