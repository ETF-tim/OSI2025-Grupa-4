#include "../../include/employee/billsManagement.hpp"

#include <fstream>
#include <iomanip>
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

void BillManager::generateBillTXTFile (int billId) {
    CSVData bills;
    CSVData workOrders;
    CSVData receiptOrders;

    try {
        workOrders = CSVData ("./data/workOrders.csv");
        receiptOrders = CSVData ("./data/receiptOrders.csv");
        bills = CSVData ("./data/bills.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        throw std::logic_error ("Neuspjesno kreiranje TXT fajla ");
    }  //------------------
    int foundBillRow = -1;
    int rowIndex = -1;

    for (rowIndex = 1; rowIndex < bills.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (bills.get_value (rowIndex, 0)) == billId) {
            foundBillRow = rowIndex;
            break;
        }
    }
    if (foundBillRow == -1) {
        std::cerr << "Racun sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------
    std::string WorkOrderId = bills.get_value (foundBillRow, 1);

    int foundWorkOrderRow = -1;

    for (rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (workOrders.get_value (rowIndex, 0)) == std::stoi (WorkOrderId)) {
            foundWorkOrderRow = rowIndex;
            break;
        }
    }
    if (foundWorkOrderRow == -1) {
        std::cerr << "Radni nalog sa zadatim  ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    int foundReceiptOrderRow = -1;
    std::string receiptOrderId = workOrders.get_value (foundWorkOrderRow, 1);
    for (rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (receiptOrders.get_value (rowIndex, 0)) == std::stoi (receiptOrderId)) {
            foundReceiptOrderRow = rowIndex;
            break;
        }
    }
    if (foundReceiptOrderRow == -1) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    std::time_t currentDate = std::time (nullptr);
    std::string opisPopravke = receiptOrders.get_value (foundReceiptOrderRow, 3);

    std::string fileName = "racun_" + std::to_string (billId);

    std::ofstream file ("./Bills/" + fileName + ".txt");

    if (!file) {
        std::cout << "Nije moguce kreirati fajl!\n";
        return;
    }
    file << "====== FISKALNI RACUN ======\n";

    file << "Datum: " << std::put_time (std::localtime (&currentDate), "%Y-%m-%d %H:%M:%S") << "\n";
    file << "Opis popravke " << opisPopravke << "\n";
    file << "1:Cijena usluge " << bills.get_value (foundBillRow, 3) << "KM\n";
    file << "2: Rad servisa:" << "20 KM" << "\n";
    file << "Ukupno za platiti: " << std::fixed << std::setprecision (2) << (std::stod (bills.get_value (foundBillRow, 3)) + 20.0)
         << " KM\n";
    file << "Nacin placanja: " << bills.get_value (foundBillRow, 2) << "\n";
    file << "\nKraj fiskalnog racuna\n";
    file << "\nHvala na povjerenju!\n";
    file.close ();
    std::cout << "Fajl uspjesno kreiran:" << fileName << "\n";
}

void BillManager::mainBillsManagement () {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n----- MENADZMENT RACUNA -----\n";
        std::cout << "1. Prikaz liste racuna\n";
        std::cout << "2. Brisanje racuna\n";
        std::cout << "0. Za povratak na izborni meni\n";
        std::cout << "-----------------------------\n";
        std::cout << "Unesite izbor: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                listBills ();
                break;
            case 2:
                deleteBill ();
                break;

            case 0:
                break;

            default:
                std::cout << "Pogresan izbor. Pokusajte ponovo.\n";
                break;
        }
    }
}
