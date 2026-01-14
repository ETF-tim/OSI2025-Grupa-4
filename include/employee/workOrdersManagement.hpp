#pragma once
#include <map>

#include "../../include/employee/partsManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"

enum class WorkOrderStatus;

class WorkOrderManager {
    private:
        ReceiptOrderManager& receiptOrderManager;
        PartManager& partManager;

    public:
        WorkOrderManager (ReceiptOrderManager& receiptOrderManager, PartManager& partManager);
        void createWorkOrder (int);
        void listWorkOrders ();
        void updateWorkOrders ();
        void deleteWorkOrder ();
        bool searchForWorkOrder (int workOrderId);
        void listCompletedWorkOrders ();
        bool isWorkOrderCompleted (int workOrderId);
        void generateWorkOrderTXTFile (int workOrderId);
        double calculateTotalPrice (int workOrderId);

        void mainWorkOrdersManagement (int id);

    private:
        std::string statusToString (WorkOrderStatus status);
        std::string partsToString (const std::map<int, int>& parts);
        std::map<int, int> stringToParts (const std::string& s);
};