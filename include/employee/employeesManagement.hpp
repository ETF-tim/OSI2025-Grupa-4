#pragma once
#include <string>

class EmployeeManager {
    public:
        void createEmployee ();
        void listEmployees ();
        void editEmployee ();
        void deleteEmployee ();
        bool searchForEmployee (int employeeId);

        bool searchForEmployeeByEmail (std::string& email);
        int getEmployeePINByEmail (std::string& email);
        int getEmployeeIdByEmail (std::string& email);
        std::string getEmployeeRoleByEmail (std::string& email);

        void mainEmployeesManagement ();

    private:
        std::string roleToString (int roleInt);
};