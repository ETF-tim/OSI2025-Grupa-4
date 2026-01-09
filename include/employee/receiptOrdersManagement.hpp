#pragma once
#include "../../include/employee/devicesManagement.hpp"
#include "../../include/employee/usersManagement.hpp"

class ReceiptOrderManager {
    private:
        UserManager& userManager;
        DeviceManager& deviceManager;

    public:
        ReceiptOrderManager (UserManager& userManager, DeviceManager& deviceManager);
        void createReceiptOrder ();
        void listReceiptOrders ();
        void deleteReceiptOrder ();
        bool searchForReceiptOrder (int receiptOrderId);
};