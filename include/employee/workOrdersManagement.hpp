#pragma once
#include "../../include/employee/partsManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"

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
};