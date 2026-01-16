#include <cstdlib>
#include <iostream>
#include <string>

#include "../include/client/deviceRepairStatusManagement.hpp"
#include "../include/common/authentication.hpp"
#include "../include/common/personalDataManagement.hpp"
#include "../include/common/util/Validate.hpp"
#include "../include/employee/employeesManagement.hpp"
#include "../include/employee/usersManagement.hpp"
//-----------------------------------------------------------------------
EmployeeManager employeeManager;
UserManager userManager;
DeviceRepairStatusManager deviceRepairStatusManager;

PersonalDataManager personalDataManager (userManager, employeeManager);
Authentication auth (employeeManager, userManager);

int id = -1;
std::string role;
//-----------------------------------------------------------------------

void clientMainMenu () {
    while (true) {
#ifdef _WIN32
        system ("cls");  // Windows
#else
        system ("clear");  // Linux / macOS
#endif
        std::cout << "====================================" << std::endl;
        std::cout << "           KLIJENTSKI PANEL         " << std::endl;
        std::cout << "====================================" << std::endl;
        std::cout << "Izaberite jednu od opcija:" << std::endl;
        std::cout << "1. Pregled statusa uredjaja" << std::endl;
        std::cout << "2. Upravljanje licnim podacima" << std::endl << std::endl;

        std::cout << "0. Odjava sa sistema" << std::endl;

        std::string choiceString;
        int choice;
        do {
            std::cout << "Unesite vas izbor (0-2): ";
            std::getline (std::cin, choiceString);
        } while (!Validate::isValidInteger (choiceString) || (choice = std::stoi (choiceString)) < 0 || choice > 2);

        switch (choice) {
            case 1:
                deviceRepairStatusManager.checkRepairStatus (id);
                break;
            case 2:
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
    std::cout << "              UNISERV.APP           " << std::endl;
    std::cout << "     Dobrodosli u nasu aplikaciju   " << std::endl;
    std::cout << "====================================" << std::endl;
    std::cout << "Prijava na sistem:" << std::endl;
}

int main () {
    while (true) {
        printHomePage ();

        while (auth.login (false, id, role) == false) {
            std::cout << "Pokusajte ponovo." << std::endl;
            std::cout << "-------------------------------" << std::endl;
        }
        clientMainMenu ();
        auth.logout (id, role);
    }
    return 0;
}
