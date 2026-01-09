#include "../../include/employee/billsManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Bill.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

std::string BillManager::paymentMethodToString (int paymentMethodInt) {
    if (paymentMethodInt == 1)
        return "CASH";
    else if (paymentMethodInt == 2)
        return "CREDIT_CARD";
    else if (paymentMethodInt == 3)
        return "DIGITAL_WALLET";
    else
        return "UNKNOWN";
}

void BillManager::createBill (int workOrderID, PaymentMethod paymentMethod, double price) {
    // Opening CSV file
    CSVData bills;
    try {
        bills = CSVData ("./data/bills.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog racuna";
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = bills.get_row (0);
    bills.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (bills);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog racuna";
        return;
    }  //------------------

    // Re-add header and new bill to CSV data
    bills.add_row (header, 0);  // Re-add header row
    bills.add_row ({std::to_string (tempId), std::to_string (workOrderID), paymentMethodToString (static_cast<int> (paymentMethod)),
                    std::to_string (price)});
    //------------------

    // Writing updated data back to CSV file
    bills.write_data ("./data/bills.csv");
    //------------------

    std::cout << "Uspjesno kreiran novi racun!" << std::endl;
}

void BillManager::listBills () {
    // Opening CSV file
    CSVData bills;
    try {
        bills = CSVData ("./data/bills.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste racuna";
        return;
    }  //------------------

    // Store and remove header row
    std::vector<std::string> header = bills.get_row (0);
    bills.delete_row (0);  // Remove header row
    //------------------

    // Sorting CSV data by part ID in ascending order
    bills.sort_by_col (0, CSVData::ASC);
    //------------------

    // Re-add header row
    bills.add_row (header, 0);  // Re-add header row
    //------------------

    // Print parts list
    std::cout << "----- LISTA RACUNA -----" << std::endl;
    bills.print_csv_data ();
}

void BillManager::deleteBill () {
    // Opening CSV file
    CSVData bills;
    try {
        bills = CSVData ("./data/bills.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje racuna";
        return;
    }  //------------------

    // Pring parts list
    listBills ();
    //------------------

    // Choose bill to delete by ID and check if it exists
    // -> Choose bill to delete by ID
    std::cout << "Unesite ID racuna koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool billFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < bills.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (bills.get_value (rowIndex, 0)) == deleteId) {
            billFound = true;
            break;
        }
    }
    if (!billFound) {
        std::cerr << "Racun sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete part from CSV data
    bills.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    bills.write_data ("./data/bills.csv");

    // Print updated bills list
    listBills ();
    std::cout << "Uspjesno obrisan racun!" << std::endl;
}

bool BillManager::searchForBill (int billId) {
    // Opening CSV file
    CSVData bills;
    try {
        bills = CSVData ("./data/bills.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje racuna";
        return false;
    }  //------------------

    // Check if bill exists
    for (int rowIndex = 1; rowIndex < bills.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (bills.get_value (rowIndex, 0)) == billId) {
            return true;
        }
    }
    return false;
}
