#include "../../include/employee/workOrdersManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/WorkOrder.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/partsManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"

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

    // select which parts will be used during the repair
    std::map<int, int> usedParts;

    while (true) {
        std::cout << "Dodaj dio u popravku? (d/n):";
        char choice;
        std::cin >> choice;
        if (choice == 'n') break;  // If input is 'n', exit from the loop

        int partId, quantity;

        std::cout << "Unesite id dijela koji zelite dodati:";
        std::cin >> partId;
        std::cout << "Unesite kolicinu dijela koji zelite uzeti sa skladista:";
        std::cin >> quantity;
        std::cin.ignore ();  // Clear newline from buffer

        // Check 1: does part exist
        if (!partManager.searchForPart (partId)) {
            std::cout << "Dio sa unesenim ID-em nije pronadjen." << std::endl;
            continue;
        }

        // Check 2: is part available with this quantity
        if (!partManager.isPartAvailable (partId, quantity)) {
            std::cout << "Nema dovoljno delova u skladistu" << std::endl;
            continue;
        }

        partManager.decreasePartStock (partId, quantity);

        usedParts[partId] += quantity;
        std::cout << "Dio je dodan u popravku!" << std::endl;
    }
