#pragma once

#include <iostream>
#include <string>

#include "../employee/employeesManagement.hpp"
#include "../employee/usersManagement.hpp"

class Authentication {
        EmployeeManager& employeeManager;
        UserManager& userManager;

    public:
        Authentication (EmployeeManager& employeeManager, UserManager& userManager);

        bool login (bool isEmployeeSystem, int& id, std::string& role);
        bool logout (int& id, std::string& role);
};