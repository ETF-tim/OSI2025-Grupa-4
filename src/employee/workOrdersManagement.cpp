#include "../../include/employee/workOrdersManagement.hpp"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "../../include/common/models/WorkOrder.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/partsManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"
#include "workOrdersManagement.hpp"

std::string statusToString (WorkOrderStatus status) {
    switch (status) {
        case WorkOrderStatus::IN_DIAGNOSTICS:
            return "IN_DIAGNOSTICS";
        case WorkOrderStatus::IN_REPAIR:
            return "IN_REPAIR";
        case WorkOrderStatus::WAITING_FOR_PARTS:
            return "WAITING_FOR_PARTS";
        case WorkOrderStatus::COMPLETED:
            return "COMPLETED";
        default:
            return "UNKNOWN";
    }
}

std::string partsToString (const std::map<int, int>& usedParts) {
    if (usedParts.empty ()) return "";
    std::string result;
    for (const auto& p : usedParts) {
        if (!result.empty ()) result += ";";
        result += std::to_string (p.first) + ":" + std::to_string (p.second);
    }
    return result;
}

void WorkOrderManager::createWorkOrder (int technicianID) {
    // Opening CSV file
    CSVData workOrders;
    try {
        workOrders = CSVData ("./data/workOrders.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog prijemnog naloga";
        return;
    }

    // Store and remove header row
    std::vector<std::string> header = workOrders.get_row (0);
    workOrders.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (workOrders);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog radnog naloga";
        return;
    }  //------------------

    // Print list of free receipt orders
    receiptOrderManager.listFreeReceiptOrders ();

    // ----------------

    int temptReceiptOrderID;
    std::cout << "Unesite ID prijemnog naloga za koji kreirate radni nalog: ";
    std::cin >> temptReceiptOrderID;
    std::cin.ignore ();  // Clear newline character from input buffer
    // ----------------

    // Check if receipt order exists
    if (!receiptOrderManager.searchForReceiptOrder (temptReceiptOrderID)) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Check if receipt order is free
    if (!receiptOrderManager.isReceiptOrderFree (temptReceiptOrderID)) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije slobodan." << std::endl;
        return;
    }  //------------------

    // Set initial status to IN_DIAGNOSTICS when starting work on device
    WorkOrderStatus currentStatus = WorkOrderStatus::IN_DIAGNOSTICS;
    std::cout << "\n Radni nalog je kreiran. Status: IN_DIAGNOSTICS" << std::endl;
    std::cout << "Tehnicar pocinje sa dijagnostikom uredjaja...\n" << std::endl;

    // Select which parts will be used during the repair
    std::map<int, int> usedParts;

    while (true) {
        std::cout << "Dodaj dio u popravku? (d/n):";
        char choice;
        std::cin >> choice;
        std::cin.ignore ();  // Clear newline from buffer

        if (choice == 'n' || choice == 'N') break;  // If input is 'n', exit from the loop

        int partId, quantity;

        std::cout << "Unesite id dijela koji zelite dodati: ";
        std::cin >> partId;
        std::cout << "Unesite kolicinu dijela koji zelite uzeti sa skladista: ";
        std::cin >> quantity;
        std::cin.ignore ();  // Clear newline from buffer

        // Check 1: does part exist
        if (!partManager.searchForPart (partId)) {
            std::cout << "Dio sa unesenim ID-em nije pronadjen." << std::endl;
            continue;
        }

        // Check 2: is part available with this quantity
        if (!partManager.isPartAvailable (partId, quantity)) {
            std::cout << "Nema dovoljno delova u skladistu." << std::endl;
            std::cout << "Mozete promeniti status u WAITING_FOR_PARTS nakon zavrsetka unosa." << std::endl;
            continue;
        }

        partManager.decreasePartStock (partId, quantity);

        usedParts[partId] += quantity;
        std::cout << "Dio je dodan u popravku!" << std::endl;
    }
    std::cout << "Izaberite novo stanje radnog naloga, ako zelite promjeniti:" << std::endl;
    std::cout << "1. IN_DIAGNOSTICS" << std::endl;
    std::cout << "2. IN_REPAIR" << std::endl;
    std::cout << "3. WAITING_FOR_PARTS" << std::endl;
    int attributeChoice;
    do {
        std::cout << "Unesite broj stanja (1-3): ";
        std::cin >> attributeChoice;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (attributeChoice < 1 || attributeChoice > 3);

    switch (attributeChoice) {
        case 1:
            currentStatus = WorkOrderStatus::IN_DIAGNOSTICS;
            break;
        case 2:
            currentStatus = WorkOrderStatus::IN_REPAIR;
            break;
        case 3:
            currentStatus = WorkOrderStatus::WAITING_FOR_PARTS;
            break;
    }
    std::time_t startDate = std::time (nullptr);  // Current date/time
    std::time_t endDate = 0;
    double servicePrice = 0;  // Will be updated when work is COMPLETED
    std::string comment;
    int response;
    std::cout << "Da li zelite uneti komentar: (d=1/n=2)" << std::endl;
    do {
        std::cout << "Unesite odgovor: ";
        std::cin >> response;
        std::cin.ignore ();  // Clear newline character from input buffer
    } while (response < 1 || response > 2);

    if (response == 1) {
        std::cin.ignore (std::numeric_limits<std::streamsize>::max (), '\n');
        std::cout << "Unesite komentar:" << std::endl;
        std::getline (std::cin, comment);
    }

    workOrders.add_row (header, 0);  // Re-add header row
    workOrders.add_row ({std::to_string (tempId), std::to_string (temptReceiptOrderID), statusToString (currentStatus), comment,
                         std::to_string (startDate), std::to_string (endDate), std::to_string (technicianID),
                         partsToString (usedParts), std::to_string (servicePrice)});

    // Write to file
    workOrders.write_data ("./data/workOrders.csv");

    std::cout << "\nUspjesno kreiran radni nalog!" << std::endl;
    std::cout << "ID: " << tempId << std::endl;
    std::cout << "Status: " << statusToString (currentStatus) << std::endl;
}
