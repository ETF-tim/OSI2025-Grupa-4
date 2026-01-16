#include "../../include/common/personalDataManagement.hpp"

#include <iostream>
#include <string>

#include "../../include/common/util/GeneratePIN.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/employeesManagement.hpp"
#include "../../include/employee/usersManagement.hpp"

std::string roleToString (int roleInt) {
    if (roleInt == 1)
        return "RECEPTIONIST";
    else if (roleInt == 2)
        return "TECHNICIAN";
    else if (roleInt == 3)
        return "SERVICE_OWNER";
    else
        return "UNKNOWN";
}

PersonalDataManager::PersonalDataManager (UserManager& um, EmployeeManager& em) : userManager (um), employeeManager (em) {}

void PersonalDataManager::listPersonalData (int id, std::string role) {
    if (role == "CLIENT") {
        // Opening CSV file
        CSVData users;
        try {
            users = CSVData ("./data/users.csv");
        } catch (std::exception& e) {
            std::cout << e.what () << std::endl;
            std::cerr << "Neuspjesno pregledanje licnih podataka." << std::endl;
            return;
        }

        // Search for user and print user personal data
        for (int rowIndex = 1; rowIndex < users.rows (); rowIndex++) {
            if (stoi (users.get_value (rowIndex, 0)) == id) {
                std::cout << "----- PREGLED LICNIH PODATAKA -----" << std::endl;
                std::cout << "ID: " << users.get_value (rowIndex, 0) << std::endl;
                std::cout << "Ime: " << users.get_value (rowIndex, 1) << std::endl;
                std::cout << "Prezime: " << users.get_value (rowIndex, 2) << std::endl;
                std::cout << "Email: " << users.get_value (rowIndex, 3) << std::endl;
                std::cout << "Telefon: " << users.get_value (rowIndex, 4) << std::endl;
                std::cout << "------------------------------------------------" << std::endl;
                return;
            }
        }
        std::cerr << "Licni podaci nisu pronadjeni." << std::endl;
        return;
    } else {
        // Opening CSV file
        CSVData employees;
        try {
            employees = CSVData ("./data/employees.csv");
        } catch (std::exception& e) {
            std::cout << e.what () << std::endl;
            std::cerr << "Neuspjesno pregledanje licnih podataka." << std::endl;
            return;
        }

        // Search for user and print user personal data
        for (int rowIndex = 1; rowIndex < employees.rows (); rowIndex++) {
            if (stoi (employees.get_value (rowIndex, 0)) == id) {
                std::cout << "----- PREGLED LICNIH PODATAKA -----" << std::endl;
                std::cout << "ID: " << employees.get_value (rowIndex, 0) << std::endl;
                std::cout << "Ime: " << employees.get_value (rowIndex, 1) << std::endl;
                std::cout << "Prezime: " << employees.get_value (rowIndex, 2) << std::endl;
                std::cout << "Email: " << employees.get_value (rowIndex, 3) << std::endl;
                std::cout << "Telefon: " << employees.get_value (rowIndex, 4) << std::endl;
                std::cout << "Uloga: " << employees.get_value (rowIndex, 5) << std::endl;
                std::cout << "------------------------------------------------" << std::endl;
                return;
            }
        }
        std::cerr << "Licni podaci nisu pronadjeni." << std::endl;
        return;
    }
}

void PersonalDataManager::editPersonalData (int id, std::string role) {
    std::cout << "---- Trenutni podaci korisnika su ----" << std::endl;
    listPersonalData (id, role);

    if (role == "CLIENT") {
        // Opening CSV file
        CSVData users;
        try {
            users = CSVData ("./data/users.csv");
        } catch (std::exception& e) {
            std::cout << e.what () << std::endl;
            std::cerr << "Neuspjesno uredjivanje licnih podataka." << std::endl;
            return;
        }

        // -> Check if it exists
        bool userFound = false;
        int rowIndex;
        for (rowIndex = 1; rowIndex < users.rows (); rowIndex++) {  // Start from 1 to skip header row
            if (std::stoi (users.get_value (rowIndex, 0)) == id) {
                userFound = true;
                break;
            }
        }
        if (!userFound) {
            std::cerr << "Licni podaci nisu pronadjeni." << std::endl;
            return;
        }  //------------------

        // Choose attribute to edit by ID
        std::cout << "Koji atribut zelite urediti?" << std::endl;
        std::cout << "1. Ime korisnika" << std::endl;
        std::cout << "2. Prezime korisnika" << std::endl;
        std::cout << "3. Email korisnika" << std::endl;
        std::cout << "4. Broj telefona korisnika" << std::endl;
        std::cout << "5. PIN korisnika" << std::endl;

        std::string attributeChoiceString;
        int attributeChoice;
        do {
            std::cout << "Unesite broj atributa (1-5): ";
            std::getline (std::cin, attributeChoiceString);
        } while (!Validate::isValidInteger (attributeChoiceString) || (attributeChoice = std::stoi (attributeChoiceString)) < 1 ||
                 attributeChoice > 5);
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
    } else {
        // Opening CSV file
        CSVData employees;
        try {
            employees = CSVData ("./data/employees.csv");
        } catch (std::exception& e) {
            std::cout << e.what () << std::endl;
            std::cerr << "Neuspjesno uredjivanje licnih podataka." << std::endl;
            return;
        }

        // Choose employee to edit by ID and check if it exists
        // -> Check if it exists
        bool employeeFound = false;
        int rowIndex;
        for (rowIndex = 1; rowIndex < employees.rows (); rowIndex++) {  // Start from 1 to skip header row
            if (std::stoi (employees.get_value (rowIndex, 0)) == id) {
                employeeFound = true;
                break;
            }
        }
        if (!employeeFound) {
            std::cerr << "Licni podaci nisu pronadjeni." << std::endl;
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

        std::string attributeChoiceString;
        int attributeChoice;
        do {
            std::cout << "Unesite broj atributa (1-6): ";
            std::getline (std::cin, attributeChoiceString);
        } while (!Validate::isValidInteger (attributeChoiceString) || (attributeChoice = std::stoi (attributeChoiceString)) < 1 ||
                 attributeChoice > 6);
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
                std::string roleChoiceString;
                int roleChoice;
                do {
                    std::cout << "Unesite broj atributa (1-3): ";
                    std::getline (std::cin, roleChoiceString);
                } while (!Validate::isValidInteger (roleChoiceString) || (roleChoice = std::stoi (roleChoiceString)) < 1 ||
                         roleChoice > 3);
                newValue = roleChoiceString;
                break;
            }
        }  //------------------

        // Update employee attribute in CSV data
        employees.set_value (rowIndex, attributeChoice, newValue);
        //------------------

        // Writing updated data back to CSV file
        employees.write_data ("./data/employees.csv");
        //------------------
    }

    std::cout << "---- Novi podaci korisnika su ----" << std::endl;
    listPersonalData (id, role);
}

void PersonalDataManager::mainPersonalDataManagement (int id, std::string role) {
    int choice;
    do {
        std::cout << "----- UPRAVLJANJE LICNIM PODACIMA -----" << std::endl;
        std::cout << "1. Prikazi licne podatke" << std::endl;
        std::cout << "2. Izmeni licne podatke" << std::endl;
        std::cout << "0. Izlaz" << std::endl;

        std::string choiceString;
        do {
            std::cout << "Izaberite opciju (0-2): ";
            std::getline (std::cin, choiceString);
        } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 0 || choice > 2);

        switch (choice) {
            case 1:
                listPersonalData (id, role);
                break;
            case 2:
                editPersonalData (id, role);
                break;
            case 0:
                std::cout << "Izlaz iz upravljanja licnim podacima." << std::endl;
                break;
        }
    } while (choice != 0);
}
