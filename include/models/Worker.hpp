#include "Person.hpp"
#pragma once

enum class Role
{
RECEPCIONER,
TECHNICIAN,
SERVICE_OWNER
};

class Worker:public Person
{
private:
Role role;
public:
Worker()=default;
Worker(int id,const std::string& fn,const std::string& ln,const std::string& email,int pin,Role r);
Role getRole() const noexcept; 
void setRole(Role v); 
};

bool isValidRole(Role role);