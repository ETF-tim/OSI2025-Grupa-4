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
        bool generateWorkOrderTXTFile (int workOrderId);

    private:
        std::string statusToString (WorkOrderStatus status);
        std::string partsToString (const std::map<int, int>& parts);
};