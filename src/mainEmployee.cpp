#include <cstdlib>
#include <iostream>
#include <string>

#include "../include/common/authentication.hpp"
#include "../include/common/personalDataManagement.hpp"
#include "../include/common/util/Validate.hpp"
#include "../include/employee/billsManagement.hpp"
#include "../include/employee/deviceIntakeManagement.hpp"
#include "../include/employee/devicesManagement.hpp"
#include "../include/employee/employeesManagement.hpp"
#include "../include/employee/generalDataManagement.hpp"
#include "../include/employee/partsManagement.hpp"
#include "../include/employee/paymentManagement.hpp"
#include "../include/employee/receiptOrdersManagement.hpp"
#include "../include/employee/serviceReportsManagement.hpp"
#include "../include/employee/statisticsFunk.hpp"
#include "../include/employee/usersManagement.hpp"
#include "../include/employee/workOrdersManagement.hpp"
//-----------------------------------------------------------------------
EmployeeManager employeeManager;
UserManager userManager;
BillManager billManager;
DeviceManager deviceManager;
PartManager partManager;
ServiceReportManager serviceReportManager;
GeneralDataManager generalDataManager;
statFunk statisticsFunk;

ReceiptOrderManager receiptOrderManager (userManager, deviceManager);
DeviceIntakeManager deviceIntakeManager (userManager, deviceManager, receiptOrderManager);
WorkOrderManager workOrderManager (receiptOrderManager, partManager);
PaymentManager paymentManager (workOrderManager, billManager, serviceReportManager);

PersonalDataManager personalDataManager (userManager, employeeManager);
Authentication auth (employeeManager, userManager);

int id = -1;
std::string role;
//-----------------------------------------------------------------------

void receptionistMainMenu () {
    while (true) {
#ifdef _WIN32
        system ("cls");  // Windows
#else
        system ("clear");  // Linux / macOS
#endif
        std::cout << "====================================" << std::endl;
        std::cout << "             RECEPCIONER            " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "Izaberite jednu od opcija:" << std::endl;
        std::cout << "1. Prijem uredjaja" << std::endl;
        std::cout << "2. Upravljanje korisnickim nalozima" << std::endl;
        std::cout << "3. Upravljanje prijemnim nalozima" << std::endl;
        std::cout << "4. Pregled statistike" << std::endl;
        std::cout << "5. Naplata" << std::endl << std::endl << std::endl;

        std::cout << "6. Upravljanje licnim podacima" << std::endl << std::endl;

        std::cout << "0. Odjava sa sistema" << std::endl;

        std::string choiceString;
        int choice;
        do {
            std::cout << "Unesite vas izbor (0-6): ";
            std::getline (std::cin, choiceString);
        } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 0 || choice > 6);

        switch (choice) {
            case 1:
                deviceIntakeManager.intakeDevice ();  // uraditi
                break;
            case 2:
                userManager.mainUserManagement ();
                break;
            case 3:
                receiptOrderManager.mainReceiptOrdersManagement ();
                break;
            case 4:
                statisticsFunk.mainStat ();
                break;
            case 5:
                paymentManager.createPayment ();
                break;
            case 6:
                personalDataManager.mainPersonalDataManagement (id, role);
                break;
            case 0:
                return;
                break;
        }
    }
}
void technicianMainMenu () {
    while (true) {
#ifdef _WIN32
        system ("cls");  // Windows
#else
        system ("clear");  // Linux / macOS
#endif
        std::cout << "====================================" << std::endl;
        std::cout << "              TEHNICAR              " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "Izaberite jednu od opcija:" << std::endl;
        std::cout << "1. Upravljanje radnim nalozima" << std::endl;
        std::cout << "2. Upravljanje dijelovima" << std::endl << std::endl;

        std::cout << "3. Prijem uredjaja" << std::endl;
        std::cout << "4. Upravljanje korisnickim nalozima" << std::endl;
        std::cout << "5. Upravljanje prijemnim nalozima" << std::endl;
        std::cout << "6. Pregled statistike" << std::endl;
        std::cout << "7. Naplata" << std::endl << std::endl << std::endl;

        std::cout << "8. Upravljanje licnim podacima" << std::endl << std::endl;

        std::cout << "0. Odjava sa sistema" << std::endl;

        std::string choiceString;
        int choice;
        do {
            std::cout << "Unesite vas izbor (0-8): ";
            std::getline (std::cin, choiceString);
        } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 0 || choice > 8);

        switch (choice) {
            case 1:
                workOrderManager.mainWorkOrdersManagement (id);
                break;
            case 2:
                partManager.mainPartsManagement ();
                break;
            case 3:
                deviceIntakeManager.intakeDevice ();
                break;
            case 4:
                userManager.mainUserManagement ();
                break;
            case 5:
                receiptOrderManager.mainReceiptOrdersManagement ();
                break;
            case 6:
                statisticsFunk.mainStat ();
                break;
            case 7:
                paymentManager.createPayment ();
                break;
            case 8:
                personalDataManager.mainPersonalDataManagement (id, role);
                break;
            case 0:
                return;
                break;
        }
    }
}
void serviceOwnerMainMenu () {
    while (true) {
#ifdef _WIN32
        system ("cls");  // Windows
#else
        system ("clear");  // Linux / macOS
#endif
        std::cout << "====================================" << std::endl;
        std::cout << "           VLASNIK SERVISA          " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "Izaberite jednu od opcija:" << std::endl;

        std::cout << "1. Upravljanje podacima servisa" << std::endl;
        std::cout << "2. Upravljanje zaposlenima" << std::endl << std::endl;

        std::cout << "3. Upravljanje radnim nalozima" << std::endl;
        std::cout << "4. Upravljanje dijelovima" << std::endl << std::endl;

        std::cout << "5. Prijem uredjaja" << std::endl;
        std::cout << "6. Upravljanje korisnickim nalozima" << std::endl;
        std::cout << "7. Upravljanje prijemnim nalozima" << std::endl;
        std::cout << "8. Pregled statistike" << std::endl;
        std::cout << "9. Naplata" << std::endl << std::endl << std::endl;

        std::cout << "10. Upravljanje licnim podacima" << std::endl << std::endl;

        std::cout << "0. Odjava sa sistema" << std::endl;

        std::string choiceString;
        int choice;
        do {
            std::cout << "Unesite vas izbor (0-10): ";
            std::getline (std::cin, choiceString);
        } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 0 || choice > 10);

        switch (choice) {
            case 1:
                generalDataManager.mainGeneralDataManagement ();
                break;
            case 2:
                employeeManager.mainEmployeesManagement ();
                break;
            case 3:
                workOrderManager.mainWorkOrdersManagement (id);
                break;
            case 4:
                partManager.mainPartsManagement ();
                break;
            case 5:
                deviceIntakeManager.intakeDevice ();
                break;
            case 6:
                userManager.mainUserManagement ();
                break;
            case 7:
                receiptOrderManager.mainReceiptOrdersManagement ();
                break;
            case 8:
                statisticsFunk.mainStat ();
                break;
            case 9:
                paymentManager.createPayment ();
                break;
            case 10:
                personalDataManager.mainPersonalDataManagement (id, role);
                break;
            case 0:
                return;
                break;
        }
    }
}
//-----------------------------------------------------------------------
void printHomePage () {
#ifdef _WIN32
    system ("cls");  // Windows
#else
    system ("clear");  // Linux / macOS
#endif
    std::cout << "====================================" << std::endl;
    std::cout << "   SISTEM ZA UPRAVLJANJE SERVISOM  " << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "Dobrodosli na UniservApp!" << std::endl << std::endl;
    std::cout << "Prijava na sistem:" << std::endl;
}

int main () {
    while (true) {
        printHomePage ();

        while (auth.login (true, id, role) == false) {
            std::cout << "Pokusajte ponovo." << std::endl;
            std::cout << "-------------------------------" << std::endl;
        }
        std::cout << "Uspjesno prijavljen korisnik sa ID: " << id << " i ulogom: " << role << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl << std::endl;

        if (role == "RECEPTIONIST") {
            receptionistMainMenu ();
        } else if (role == "TECHNICIAN") {
            technicianMainMenu ();
        } else if (role == "SERVICE_OWNER") {
            serviceOwnerMainMenu ();
        }
        auth.logout (id, role);
    }
    return 0;
}
