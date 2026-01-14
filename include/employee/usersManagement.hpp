#pragma once
#include <string>

class UserManager {
    public:
        void createUser ();
        void listUsers ();
        void editUser ();
        void deleteUser ();
        bool searchForUser (int userId);

        bool searchForUserByEmail (std::string& email);
        int getUserPINByEmail (std::string& email);
        int getUserIdByEmail (std::string& email);

        void mainUserManagement ();
};