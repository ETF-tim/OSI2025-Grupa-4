#include "../../include/employee/paymentManagement.hpp"

#include <iostream>

#include "../../include/common/util/Validate.hpp"
#include "../../include/employee/usersManagement.hpp"
#include "../../include/employee/workOrdersManagement.hpp"

PaymentManager::PaymentManager (WorkOrderManager& workOrderManager, BillManager& billManager,
                                ServiceReportManager& serviceReportManager)
    : workOrderManager (workOrderManager), billManager (billManager), serviceReportManager (serviceReportManager) {}

void PaymentManager::createPayment () {
    workOrderManager.listWorkOrders ();

    std::string tempWorkOrderIDString;
    int tempWorkOrderID;
    std::cout << "Izaberite ID radnog naloga za naplatu:";
    std::getline (std::cin, tempWorkOrderIDString);

    if (!Validate::isValidInteger (tempWorkOrderIDString)) {
        std::cerr << "Pogresan unos ID-a radnog naloga." << std::endl;
        return;
    } else {
        tempWorkOrderID = std::stoi (tempWorkOrderIDString);
    }

    if (!workOrderManager.searchForWorkOrder (tempWorkOrderID)) {
        std::cerr << "Radni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }

    std::cout << "Izaberite nacin placanja:" << std::endl;
    std::cout << "1. Gotovina" << std::endl;
    std::cout << "2. Kartica" << std::endl;
    std::cout << "3. Digitalni novcanik" << std::endl;

    std::string choiceString;
    int choice;
    do {
        std::cout << "Unesite broj metode placanja (1-3): ";
        std::getline (std::cin, choiceString);
    } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 1 || choice > 3);

    PaymentMethod tempPaymentMethod = static_cast<PaymentMethod> (choice);

    double tempPrice = workOrderManager.calculateTotalPrice (tempWorkOrderID);

    int tempBillID = billManager.createBill (tempWorkOrderID, tempPaymentMethod, tempPrice);
    int tempServiceReportID = serviceReportManager.createServiceReport (tempWorkOrderID);

    billManager.generateBillTXTFile (tempBillID, tempWorkOrderID);
    serviceReportManager.generateServiceReportTXTFile (tempServiceReportID, tempBillID);

    getchar ();
}
