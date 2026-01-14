#pragma once
#include "billsManagement.hpp"
#include "serviceReportsManagement.hpp"
#include "workOrdersManagement.hpp"

enum class PaymentMethod;

class PaymentManager {
        WorkOrderManager& workOrderManager;
        BillManager& billManager;
        ServiceReportManager& serviceReportManager;

    public:
        PaymentManager (WorkOrderManager& workOrderManager, BillManager& billManager, ServiceReportManager& serviceReportManager);
        void createPayment ();
};