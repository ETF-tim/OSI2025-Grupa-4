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

void WorkOrderManager::createWorkOrder (int receiptOrderID, int technicianID) {
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

    // Check if receipt order exists
    if (!receiptOrderManager.searchForReceiptOrder (receiptOrderID)) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Check if receipt order is free
    if (!receiptOrderManager.isReceiptOrderFree (receiptOrderID)) {
        std::cerr << "Prijemni nalog sa unesenim ID-em nije slobodan." << std::endl;
        return;
    }  //------------------

    // select which parts will be used during the repair
    std::map<int, int> usedParts;

    while (true) {
        std::cout << "Dodaj dio u popravku? (d/n):";
        char choice;
        std::cin >> choice;
        if (choice == 'n') break;

        int partId, quantity;

        std::cout << "Unesite id dijela koji zelite dodati:";
        std::cin >> partId;
        std::cout << "Unesite kolicinu dijela koji zelite uzeti sa skladista:";
        std::cin >> quantity;
        if (!partManager.searchForPart (partId)) {
            std::cout << "Dio sa unesenim ID-em nije pronadjen." << std::endl;
        } else if (partManager.searchForPart (partId)) {
            if (!partManager.isPartAvailable (partId, quantity))
                std::cout << "Nema dovoljno delova u skladistu" << std::endl;
            else if (partManager.isPartAvailable (partId)) {
                partManager.editPart ();
                std::cout << "Promenjena kolicina dela sa zadatim id-em u skladistu " << std::endl;
                // potrebno promeniti mapu usedParts
            }
        }
    }