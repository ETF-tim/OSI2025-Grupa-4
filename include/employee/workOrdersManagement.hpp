#pragma once
#include "../../include/employee/partsManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"

class WorkOrderManager {
    private:
        ReceiptOrderManager& receiptOrderManager;
        PartManager& partManager;

    public:
        WorkOrderManager (ReceiptOrderManager& receiptOrderManager, PartManager& partManager);
        void createWorkOrder (int receiptOrderID, int technicianID);
        void listWorkOrders ();
        void deleteWorkOrder ();
        bool searchForWorkOrder (int workOrderId);
};