#pragma once
#include <string>

class Person
{
protected:
int id;
std::string firstname;
std::string lastname;
std::string email;
int pin;

public:
Person() = default;
 Person(int id, const std::string& fn, const std::string& ln, const std::string& email,int PIN);


    int getId()const noexcept ;
    void setId(int v) ;

    const std::string& getFirstName() const noexcept;
    void setFirstName(const std::string& v) ; 

    const std::string& getLastName() const noexcept; 
    void setLastName(const std::string& v) ; 

    const std::string& getEmail() const noexcept;
    void setEmail(const std::string& v);

    
    int getPin() const noexcept ; 
    void setPin(int v) ; 
};

bool isValidId(int id);
bool isValidFirstName(const std::string& firstName);
bool isValidLastName(const std::string& lastName);
bool isValidEmail(const std::string& email);
bool isvalidPin(int pin);