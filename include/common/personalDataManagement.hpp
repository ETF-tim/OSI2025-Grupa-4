#pragma once
#include "../employee/employeesManagement.hpp"
#include "../employee/usersManagement.hpp"

class PersonalDataManagement {
        UserManager& userManager;
        EmployeeManager& employeeManager;

    public:
        PersonalDataManagement (UserManager& um, EmployeeManager& em);

        void listPersonalData (int id, std::string role);
        void editPersonalData (int id, std::string role);
};