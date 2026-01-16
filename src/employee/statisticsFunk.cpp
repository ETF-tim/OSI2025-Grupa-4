
#include "../../include/employee/statisticsFunk.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Bill.hpp"
#include "../../include/common/models/Device.hpp"
#include "../../include/common/models/Part.hpp"
#include "../../include/common/models/Person.hpp"
#include "../../include/common/models/Receipt_order.hpp"
#include "../../include/common/models/WorkOrder.hpp"
#include "../../include/common/models/Worker.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

void statFunk::ispisZaradjenogNovca () {
    CSVData bills;
    try {
        bills = CSVData ("./data/bills.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste racuna";
        return;
    }

    std::vector<std::string> header = bills.get_row (0);
    bills.delete_row (0);

    double totalRevenue = 0.0;
    for (int rowIndex = 0; rowIndex < bills.rows (); rowIndex++) {
        totalRevenue += std::stod (bills.get_value (rowIndex, 3));
    }

    bills.add_row (header, 0);

    std::cout << "Ukupno zarađeno: " << totalRevenue << " KM" << std::endl;
}

void statFunk::ispisBrojaServisiranihUredjajaPoTipuUredjaja () {
    CSVData workOrders;
    CSVData devices;
    CSVData receiptOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
        receiptOrders = CSVData ("./data/receiptOrders.csv");
        devices = CSVData ("./data/devices.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste radnih naloga ili uredjaja";
        return;
    }

    std::vector<std::string> headerWO = workOrders.get_row (0);
    workOrders.delete_row (0);

    std::vector<std::string> headerD = devices.get_row (0);
    devices.delete_row (0);

    std::vector<std::string> headerRO = receiptOrders.get_row (0);
    receiptOrders.delete_row (0);

    std::map<std::string, int> deviceTypeCount;

    for (int rowIndex = 0; rowIndex < workOrders.rows (); rowIndex++) {
        std::string receiptOrderId = workOrders.get_value (rowIndex, 1);

        std::string deviceIMEI;
        for (int roRowIndex = 0; roRowIndex < receiptOrders.rows (); roRowIndex++) {
            if (receiptOrders.get_value (roRowIndex, 0) == receiptOrderId) {
                deviceIMEI = receiptOrders.get_value (roRowIndex, 2);
                break;
            }
        }

        for (int dRowIndex = 0; dRowIndex < devices.rows (); dRowIndex++) {
            if (devices.get_value (dRowIndex, 3) == deviceIMEI) {           // Assuming device ID is in column 0
                std::string deviceType = devices.get_value (dRowIndex, 1);  // Assuming device type is in column 1
                std::cout << "---" << deviceType << " " << deviceIMEI << std::endl;
                deviceTypeCount[deviceType]++;
                break;
            }
        }
    }

    devices.add_row (headerD, 0);
    workOrders.add_row (headerWO, 0);

    std::cout << "Broj servisiranih uredjaja po tipu uredjaja:\n";
    for (const auto& pair : deviceTypeCount) {
        std::cout << "Tip uredjaja: " << pair.first << " - Broj servisiranih: " << pair.second << "\n";
    }
}

void statFunk::ispisUspjesnostiServisnihTehnicara () {
    CSVData workOrders;
    CSVData worker;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
        worker = CSVData ("./data/employees.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste radnih naloga";
        return;
    }
    std::vector<std::string> headerWO = workOrders.get_row (0);
    workOrders.delete_row (0);
    std::vector<std::string> headerW = worker.get_row (0);
    worker.delete_row (0);

    std::map<std::string, std::pair<int, int>> technicianStats;  // <Technician Name, <Completed, Total>>

    for (int rowIndex = 0; rowIndex < workOrders.rows (); rowIndex++) {
        std::string technicianId = workOrders.get_value (rowIndex, 6);  // Assuming technician ID is in column 6
        std::string status = workOrders.get_value (rowIndex, 2);        // Assuming status is in column 2

        std::string technicianName;
        for (int wRowIndex = 0; wRowIndex < worker.rows (); wRowIndex++) {
            if (worker.get_value (wRowIndex, 0) == technicianId) {  // Assuming worker ID is in column 0
                technicianName = worker.get_value (wRowIndex, 1);   // Assuming worker name is in column 1
                break;
            }
        }

        technicianStats[technicianName].second++;  // Increment total count
        if (status == "COMPLETED") {
            technicianStats[technicianName].first++;  // Increment completed count
        }
    }

    for (const auto& pair : technicianStats) {
        const std::string& name = pair.first;
        int completed = pair.second.first;
        int total = pair.second.second;
        double successRate = (total > 0) ? (static_cast<double> (completed) / total) * 100.0 : 0.0;

        std::cout << "Tehnicar: " << name << " - Uspjesnost: " << successRate << "% (" << completed << "/" << total << ")\n";
    }
}

void statFunk::ispisZaradaPoServisnomTehnicaru () {
    CSVData workOrders;
    CSVData bills;
    CSVData worker;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
        bills = CSVData ("./data/bills.csv");
        worker = CSVData ("./data/employees.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste radnih naloga ili racuna";
        return;
    }

    std::vector<std::string> headerWO = workOrders.get_row (0);
    workOrders.delete_row (0);

    std::vector<std::string> headerB = bills.get_row (0);
    bills.delete_row (0);

    std::vector<std::string> headerW = worker.get_row (0);
    worker.delete_row (0);

    std::map<std::string, double> technicianEarnings;

    for (int rowIndex = 0; rowIndex < bills.rows (); rowIndex++) {
        std::string workOrderId = bills.get_value (rowIndex, 1);        // Assuming work order ID is in column 1
        double billAmount = std::stod (bills.get_value (rowIndex, 3));  // Assuming bill amount is in column 3
        // std::cout<<billAmount<<"   ";
        std::string technicianId;
        for (int wRowIndex = 0; wRowIndex < workOrders.rows (); wRowIndex++) {
            if (workOrders.get_value (wRowIndex, 0) == workOrderId) {  // Assuming work order ID is in column 0
                technicianId = workOrders.get_value (wRowIndex, 6);    // Assuming technician ID is in column 6
                break;
            }
        }
        // std::cout<<technicianId<<"   ";
        std::string technicianName;
        for (int wRowIndex = 0; wRowIndex < worker.rows (); wRowIndex++) {
            if (worker.get_value (wRowIndex, 0) == technicianId) {  // Assuming worker ID is in column 0
                technicianName = worker.get_value (wRowIndex, 1);   // Assuming worker name is in column 1
                break;
            }
        }
        // std::cout<<technicianName<<"   "<<std::endl;
        technicianEarnings[technicianName] += billAmount;
    }

    workOrders.add_row (headerWO, 0);
    bills.add_row (headerB, 0);
    worker.add_row (headerW, 0);
    std::cout << "Zarada po servisnom tehnicaru:\n";
    for (const auto& pair : technicianEarnings) {
        std::cout << "Tehnicar: " << pair.first << " - Zarada: " << pair.second << " KM\n";
    }
}

void statFunk::mainStat () {
    int choice;
    do {
        std::cout << "-----STATISTIKE-----" << std::endl;
        std::cout << "1. Ispis zaradjenog novca" << std::endl;
        std::cout << "2. Ispis broja servisiranih uredjaja po tipu uredjaja" << std::endl;
        std::cout << "3. Ispis uspjesnosti servisnih tehnicara" << std::endl;
        std::cout << "4. Ispis zarada po servisnom tehnicaru" << std::endl;
        std::cout << "0. Povratak na prethodni meni" << std::endl;

        std::string choiceString;
        do {
            std::cout << "Izaberite opciju (0-4): ";
            std::getline (std::cin, choiceString);
        } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 0 || choice > 4);

        switch (choice) {
            case 1:
                ispisZaradjenogNovca ();
                break;
            case 2:
                ispisBrojaServisiranihUredjajaPoTipuUredjaja ();
                break;
            case 3:
                ispisUspjesnostiServisnihTehnicara ();
                break;
            case 4:
                ispisZaradaPoServisnomTehnicaru ();
                break;
            case 0:
                std::cout << "Povratak na prethodni meni..." << std::endl;
                break;
        }
    } while (choice != 0);
}
