#pragma once
#include <string>

#include "../../include/employee/devicesManagement.hpp"
#include "../../include/employee/receiptOrdersManagement.hpp"
#include "../../include/employee/usersManagement.hpp"
class DeviceIntakeManager {
        UserManager& userManager;
        DeviceManager& deviceManager;
        ReceiptOrderManager& receiptOrderManager;

    public:
        DeviceIntakeManager (UserManager& um, DeviceManager& dm, ReceiptOrderManager& rom);
        void intakeDevice ();
};
