#include "../../include/employee/generalDataManagement.hpp"

#include <iostream>
#include <stdexcept>

#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

void GeneralDataManager::listGeneralData () {
    // Opening CSV file
    CSVData generalData;
    try {
        generalData = CSVData ("./data/generalData.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje podataka servisa";
        return;
    }  //------------------

    // Print device list
    std::cout << "----- PODACI SERVISA -----" << std::endl;
    std::cout << "Naziv servisa: " << generalData.get_value (1, 0) << std::endl;
    std::cout << "Adresa servisa: " << generalData.get_value (1, 1) << std::endl;
    std::cout << "Email servisa: " << generalData.get_value (1, 2) << std::endl;
    std::cout << "Kontakt telefon: " << generalData.get_value (1, 3) << std::endl;
    std::cout << "JIB: " << generalData.get_value (1, 4) << std::endl;
    std::cout << "----------------------------" << std::endl << std::endl;
}
void GeneralDataManager::editGeneralData () {
    // Opening CSV file
    CSVData generalData;
    try {
        generalData = CSVData ("./data/generalData.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno azuriranje podataka servisa";
        return;
    }  //------------------

    // Choose attribute to edit
    std::cout << "Koji podatak servisa zelite urediti?" << std::endl;
    std::cout << "1. Naziv servisa" << std::endl;
    std::cout << "2. Adresa servisa" << std::endl;
    std::cout << "3. Email servisa" << std::endl;
    std::cout << "4. Kontakt telefon" << std::endl;
    std::cout << "5. JIB" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj podatka (1-5): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 5);

    // Input new value for chosen attribute
    std::string newValue;
    switch (attributeChoice) {
        case 1:
            do {
                std::cout << "Unesite novi naziv servisa: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidServiceName (newValue));
            break;
        case 2:
            do {
                std::cout << "Unesite novu adresu servisa: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidServiceAddress (newValue));
            break;
        case 3:
            do {
                std::cout << "Unesite novi email servisa: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidEmail (newValue));
            break;
        case 4:
            do {
                std::cout << "Unesite novi kontakt telefon: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidPhone (newValue));
            break;
        case 5:
            do {
                std::cout << "Unesite novi JIB: ";
                std::getline (std::cin, newValue);
            } while (!Validate::isValidJIB (newValue));
            break;
    }

    // Update attribute in CSV data
    generalData.set_value (1, attributeChoice - 1, newValue);
    //------------------

    // Writing updated data back to CSV file
    generalData.write_data ("./data/generalData.csv");
    //------------------

    // Print updated general data
    listGeneralData ();
    std::cout << "Uspjesno uređen podatak servisa!" << std::endl;
}

void GeneralDataManager::mainGeneralDataManagement () {
    int choice;
    do {
        std::cout << "----- UPRAVLJANJE PODACIMA SERVISA -----" << std::endl;
        std::cout << "1. Prikazi podatke servisa" << std::endl;
        std::cout << "2. Uredi podatke servisa" << std::endl;
        std::cout << "0. Izlaz iz upravljanja podacima servisa" << std::endl;
        std::cout << "Unesite vas izbor (0-2): ";
        std::cin >> choice;
        std::cin.ignore ();  // Clear newline character from input buffer

        switch (choice) {
            case 1:
                listGeneralData ();
                break;
            case 2:
                editGeneralData ();
                break;
            case 0:
                std::cout << "Izlaz iz upravljanja podacima servisa." << std::endl;
                break;
            default:
                std::cout << "Pogresan unos. Pokusajte ponovo." << std::endl;
        }
    } while (choice != 0);
}