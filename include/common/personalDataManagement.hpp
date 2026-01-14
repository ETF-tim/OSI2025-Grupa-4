#pragma once
#include "../employee/employeesManagement.hpp"
#include "../employee/usersManagement.hpp"

class PersonalDataManager {
        UserManager& userManager;
        EmployeeManager& employeeManager;

    public:
        PersonalDataManager (UserManager& um, EmployeeManager& em);

        void listPersonalData (int id, std::string role);
        void editPersonalData (int id, std::string role);
};