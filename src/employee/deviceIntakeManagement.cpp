#include "../../include/employee/deviceIntakeManagement.hpp"

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

DeviceIntakeManager::DeviceIntakeManager (UserManager& um, DeviceManager& dm, ReceiptOrderManager& rom)
    : userManager (um), deviceManager (dm), receiptOrderManager (rom) {}

void DeviceIntakeManager::intakeDevice () {
    std::cout << "Prijem uredjaja na servis" << std::endl;
    std::cout << "--------------------------" << std::endl;

    std::cout << "Da li je potrebno kreirati novog korisnika? (d/n): ";
    char createNewUserChoice;
    do {
        std::cin >> createNewUserChoice;

    } while (createNewUserChoice != 'd' && createNewUserChoice != 'D' && createNewUserChoice != 'n' && createNewUserChoice != 'N');
    if (createNewUserChoice == 'd' || createNewUserChoice == 'D') {
        userManager.createUser ();
    }
    std::cout << "--------------------------" << std::endl;

    std::cout << "Da li je potrebno kreirati novi uredjaj? (d/n): ";
    char createNewDeviceChoice;
    do {
        std::cin >> createNewDeviceChoice;

    } while (createNewDeviceChoice != 'd' && createNewDeviceChoice != 'D' && createNewDeviceChoice != 'n' &&
             createNewDeviceChoice != 'N');
    if (createNewDeviceChoice == 'd' || createNewDeviceChoice == 'D') {
        deviceManager.createDevice ();
    }
    std::cout << "--------------------------" << std::endl;
    std::cout << "Kreiranje naloga za prijem uredjaja na servis:" << std::endl;
    receiptOrderManager.createReceiptOrder ();
}