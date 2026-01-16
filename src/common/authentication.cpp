#include "../../include/common/authentication.hpp"

#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"
#include "../../include/employee/employeesManagement.hpp"
#include "../../include/employee/usersManagement.hpp"

Authentication::Authentication (EmployeeManager& employeeManager, UserManager& userManager)
    : employeeManager (employeeManager), userManager (userManager) {}

bool Authentication::login (bool isEmployeeSystem, int& id, std::string& role) {
    std::string email;
    std::cout << "Enter email: ";
    std::getline (std::cin, email);
    if (!Validate::isValidEmail (email)) {
        std::cerr << "Neuspjesna prijava." << std::endl;
        return false;
    }

    std::string pinString;
    int pin;
    std::cout << "Enter PIN: ";
    std::getline (std::cin, pinString);
    if (!Validate::isValidInteger (pinString) || !Validate::isValidPin (pin)) {
        std::cerr << "Neuspjesna prijava." << std::endl;
        return false;
    }

    if (isEmployeeSystem) {
        if (!employeeManager.searchForEmployeeByEmail (email)) {
            std::cerr << "Neuspjesna prijava." << std::endl;
            return false;
        }
        if (pin != employeeManager.getEmployeePINByEmail (email)) {
            std::cerr << "Neuspjesna prijava." << std::endl;
            return false;
        }
        id = employeeManager.getEmployeeIdByEmail (email);
        role = employeeManager.getEmployeeRoleByEmail (email);
        return true;
    } else {
        if (!userManager.searchForUserByEmail (email)) {
            std::cerr << "Neuspjesna prijava." << std::endl;
            return false;
        }
        if (pin != userManager.getUserPINByEmail (email)) {
            std::cerr << "Neuspjesna prijava." << std::endl;
            return false;
        }
        id = userManager.getUserIdByEmail (email);
        role = "CLIENT";
        return true;
    }
}

bool Authentication::logout (int& id, std::string& role) {
    id = -1;
    role = "";
    std::cout << "==============================" << std::endl << std::endl;
    std::cout << " Uspjesno odjavljen korisnik  " << std::endl;
    std::cout << "==============================" << std::endl << std::endl;
    return true;
}
