#include "../../include/models/Client.hpp"
#include <stdexcept>
#include <limits>

Client::Client(int id,const std::string& fn,const std::string& ln,const std::string& email,
   int pin): Person(id,fn,ln,email,pin){}