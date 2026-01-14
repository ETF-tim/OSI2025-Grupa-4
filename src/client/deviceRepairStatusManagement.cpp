#include "../../include/client/deviceRepairStatusManagement.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "../../include/common/util/csv_data_manipulator.hpp"

std::string DeviceRepairStatusManager::formatTimestamp (const std::string& timestampStr) {
    if (timestampStr == "0" || timestampStr.empty ()) return "Nije zapoceto";

    std::time_t timestamp = std::stoll (timestampStr);
    std::tm* timeinfo = std::localtime (&timestamp);

    std::ostringstream oss;
    oss << std::put_time (timeinfo, "%d.%m.%Y %H:%M:%S");
    return oss.str ();
}

void DeviceRepairStatusManager::checkRepairStatus (int userID) {
    // Opening CSV file
    CSVData users;
    try {
        users = CSVData ("./data/users.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesan pregled statusa popravke uredjaja";
        return;
    }

    bool userFound = false;
    int rowIndex;
    int foundUserRow;
    for (rowIndex = 1; rowIndex < users.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (((users.get_value (rowIndex, 0))) == std::to_string (userID)) {
            userFound = true;
            foundUserRow = rowIndex;
            break;
        }
    }
    if (!userFound) {
        std::cerr << "Korisnik ne postoji u bazi podataka." << std::endl;
        return;
    }

    std::string firstname = users.get_value (foundUserRow, 1);
    std::string lastname = users.get_value (foundUserRow, 2);
    std::string phone = users.get_value (foundUserRow, 4);

    CSVData receiptOrders;
    try {
        receiptOrders = CSVData ("./data/receiptOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno otvaranje prijemnih naloga";
        return;
    }

    std::vector<int> ReceiptOrderIDs;
    bool FoundReceiptOrder = false;
    std::vector<std::string> deskripcije;
    std::vector<std::string> uredjajiIMEI;

    for (rowIndex = 1; rowIndex < receiptOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (((receiptOrders.get_value (rowIndex, 1))) == std::to_string (userID)) {
            ReceiptOrderIDs.push_back (std::stoi (receiptOrders.get_value (rowIndex, 0)));
            uredjajiIMEI.push_back ((receiptOrders.get_value (rowIndex, 2)));
            deskripcije.push_back ((receiptOrders.get_value (rowIndex, 3)));

            FoundReceiptOrder = true;
        }
    }

    if (!FoundReceiptOrder) {
        std::cerr << "Nije pronadjen niti jedan prijemni nalog za zadatog korisnika." << std::endl;
        return;
    }

    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno otvaranje radnih naloga";
        return;
    }
    CSVData devices;
    try {
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno otvaranje baze sa uredjajima";
        return;
    }

    std::cout << "Status popravke za korisnika: " << firstname << " " << lastname << ", telefon: " << phone << "\n";

    int i = 0;
    bool FoundDevice = false;
    bool FoundWorkOrder = false;

    for (const auto& element : ReceiptOrderIDs) {
        std::cout << "Deskripcija popravke:" << deskripcije[i] << "\n";

        for (rowIndex = 1; rowIndex < devices.rows (); rowIndex++) {  // Start from 1 to skip header row
            if (((devices.get_value (rowIndex, 3))) == (uredjajiIMEI[i])) {
                std::cout << "Uredjaj:" << devices.get_value (rowIndex, 1) << "\n";
                std::cout << "Model uredjaja:" << devices.get_value (rowIndex, 2) << "\n";
                std::cout << "IMEI uredjaja:" << uredjajiIMEI[i] << "\n";

                FoundDevice = true;
                break;
            }
        }
        if (!FoundDevice) {
            std::cerr << "Nije pronadjen uredjaj u bazi podataka." << "\n";
            return;
        }
        std::cout << "\n";
        for (rowIndex = 1; rowIndex < workOrders.rows (); rowIndex++) {  // Start from 1 to skip header row
            if (std::stoi (workOrders.get_value (rowIndex, 1)) == element) {
                std::cout << "Status radnog naloga:" << workOrders.get_value (rowIndex, 2) << "\n";
                std::cout << "Datum pravljenja radnog naloga:" << formatTimestamp (workOrders.get_value (rowIndex, 4)) << "\n";

                FoundWorkOrder = true;
                break;
            }
        }

        if (!FoundWorkOrder) {
            std::cerr << "Nije pronadjen radni nalog u bazi podataka." << std::endl;
            return;
        }
        std::cout << "\n";
        FoundDevice = FoundWorkOrder = false;
        i++;
    }
}