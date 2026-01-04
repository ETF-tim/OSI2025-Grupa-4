#pragma once
#include "Person.hpp"
#include <string>

class Client:public Person
{
public:
Client()=default;
Client(int id,const std::string& fn,const std::string& ln,const std::string& email,int pin);
};