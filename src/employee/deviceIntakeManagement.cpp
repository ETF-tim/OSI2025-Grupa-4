#include "deviceIntakeManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/Client.hpp"
#include "../../include/common/models/Device.hpp"
#include "../../include/common/models/Receipt_order.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/devicesManagement.hpp"

void DeviceIntakeManager::intakeDevice () {
    std::cout << "Prijem uredjaja na servis" << std::endl;
    userManager.createUser ();
    deviceManager.createDevice ();
    receiptOrderManager.createReceiptOrder ();
}