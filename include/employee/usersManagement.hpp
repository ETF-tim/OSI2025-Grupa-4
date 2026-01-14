#pragma once

class UserManager {
    public:
        void createUser ();
        void listUsers ();
        void editUser ();
        void deleteUser ();
        bool searchForUser (int userId);
        void mainUserManagement ();
};