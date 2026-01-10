#pragma once
#include <string>

class EmployeeManager {
    public:
        void createEmployee ();
        void listEmployees ();
        void editEmployee ();
        void deleteEmployee ();
        bool searchForEmployee (int employeeId);

    private:
        std::string roleToString (int roleInt);
};