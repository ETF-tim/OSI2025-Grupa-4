#include <cstdlib>
#include <iostream>
#include <string>

#include "../include/client/deviceRepairStatusManagement.hpp"
#include "../include/common/authentication.hpp"
#include "../include/common/personalDataManagement.hpp"
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

        int choice;
        do {
            std::cout << "Unesite vas izbor: ";
            std::cin >> choice;
            std::cin.ignore ();  // Clear newline character from input buffer
        } while (choice < 0 || choice > 2);

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
