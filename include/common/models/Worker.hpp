#pragma once
#include "Person.hpp"

enum class Role
{
    UNKNOWN,
    RECEPTIONIST,
    TECHNICIAN,
    SERVICE_OWNER
};

class Worker : public Person
{
private:
    Role role;

public:
    Worker();
    Worker(int id, const std::string &firstname, const std::string &lastname, const std::string &email, const std::string &phone, int pin, Role role);

    Role getRole() const;

    void setRole(Role v);
};

bool isValidRole(Role role);