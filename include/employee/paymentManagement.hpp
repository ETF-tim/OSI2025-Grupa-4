#pragma once
#include "billsManagement.hpp"
#include "serviceReportsManagement.hpp"
#include "workOrdersManagement.hpp"

enum class PaymentMethod;

class PaymentManagement {
        WorkOrderManager& workOrderManager;
        BillManager& billManager;
        ServiceReportManager& serviceReportManager;

    public:
        PaymentManagement (WorkOrderManager& workOrderManager, BillManager& billManager, ServiceReportManager& serviceReportManager);
        void createPayment ();
};