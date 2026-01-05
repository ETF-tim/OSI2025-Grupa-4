#pragma once
#include <string>

class Person {
    protected:
        int id;
        std::string firstname;
        std::string lastname;
        std::string email;
        std::string phone;
        int pin;

    public:
        Person ();
        Person (int id, const std::string& firstname, const std::string& lastname, const std::string& email, const std::string& phone,
                int pin);

        int getId () const;
        const std::string& getFirstName () const;
        const std::string& getLastName () const;
        const std::string& getEmail () const;
        const std::string& getPhone () const;
        int getPin () const;

        void setId (int v);
        void setFirstName (const std::string& v);
        void setLastName (const std::string& v);
        void setEmail (const std::string& v);
        void setPhone (const std::string& v);
        void setPin (int v);
};

bool isValidId (int id);
bool isValidFirstName (const std::string& firstName);
bool isValidLastName (const std::string& lastName);
bool isValidEmail (const std::string& email);
bool isValidPhone (const std::string& phone);
bool isValidPin (int pin);