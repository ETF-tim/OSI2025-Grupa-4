#include "../../include/models/Worker.hpp"
#include <stdexcept>
#include <limits>

Worker::Worker(int id,const std::string& fn,const std::string& ln,const std::string& email,
   int pin,Role r): Person(id,fn,ln,email,pin),role(r){}
   
Role Worker::getRole() const noexcept 
{
 return role;
 }

void Worker::setRole(Role v) 
 { 
    if(!isValidRole(v))
        throw std::invalid_argument("Neodgovarajuca uloga");
    role = v; 
}

bool isValidRole(Role role)
{
    switch(role)
    {
        case Role::RECEPCIONER:
        case Role::TECHNICIAN:
        case Role::SERVICE_OWNER:
            return true;
        default:
            return false;
    }
}