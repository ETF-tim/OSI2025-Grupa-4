#include "../../include/employee/paymentManagement.hpp"

#include <iostream>

#include "../../include/employee/usersManagement.hpp"
#include "../../include/employee/workOrdersManagement.hpp"

PaymentManagement::PaymentManagement (WorkOrderManager& workOrderManager, BillManager& billManager,
                                      ServiceReportManager& serviceReportManager)
    : workOrderManager (workOrderManager), billManager (billManager), serviceReportManager (serviceReportManager) {}

void PaymentManagement::createPayment () {
    workOrderManager.listWorkOrders ();

    std::cout << "Izaberite ID radnog naloga za naplatu:";
    int tempWorkOrderID;
    std::cin >> tempWorkOrderID;
    std::cin.ignore ();

    if (!workOrderManager.searchForWorkOrder (tempWorkOrderID)) {
        std::cerr << "Radni nalog sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }

    std::cout << "Izaberite nacin placanja:" << std::endl;
    std::cout << "1. Gotovina" << std::endl;
    std::cout << "2. Kartica" << std::endl;
    std::cout << "3. Digitalni novcanik" << std::endl;
    int choise;
    do {
        std::cin >> choise;
        std::cin.ignore ();
    } while (choise < 1 || choise > 3);

    PaymentMethod tempPaymentMethod = static_cast<PaymentMethod> (choise);

    double tempPrice = workOrderManager.calculateTotalPrice (tempWorkOrderID);

    billManager.createBill (tempWorkOrderID, tempPaymentMethod, tempPrice);
    int tempServiceReportID = serviceReportManager.createServiceReport (tempWorkOrderID);

    billManager.generateBillTXTFile (tempWorkOrderID);
    serviceReportManager.generateServiceReportTXTFile (tempServiceReportID);
}
