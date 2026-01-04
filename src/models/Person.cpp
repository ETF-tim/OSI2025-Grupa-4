#include "../../include/models/Client.hpp"
#include <stdexcept>
#include <limits>


Person::Person(int id, const std::string& fn, const std::string& ln, const std::string& email, int PIN)
    : id(id), firstname(fn), lastname(ln), email(email), pin(PIN){}

int Person::getId() const noexcept
{
    return id;
}

void Person::setId(int v)
{
   if(!isValidId(v))
        throw std::invalid_argument("Neodgovarajuci id");
        id = v;
    
}


const std::string& Person::getFirstName() const noexcept
{
    return firstname;
}

void Person::setFirstName(const std::string& v)
{
 if(!isValidFirstName(v))
        throw std::invalid_argument("Neodgovarajuce ime");
        firstname = v;
    
}

const std::string& Person::getLastName() const noexcept
{
    return lastname;
}

void Person::setLastName(const std::string& v)
{
    if(!isValidLastName(v))
        throw std::invalid_argument("Neodgovarajuce prezime");
        lastname = v;
    
}


const std::string& Person::getEmail() const noexcept
{
    return email;
}

void Person::setEmail(const std::string& v)
{
 if(!isValidEmail(v))
        throw std::invalid_argument("Neodgovarajuci email");
        email = v;
    
}


int Person::getPin() const noexcept
{
    return pin;
}

void Person::setPin(int v)
{ if(!isValidPin(v))
        throw std::invalid_argument("Neodgovarajuci pin");
        pin = v;
    
}

bool isValidId(int id)
{
    return id > 0;
}

bool isValidFirstName(const std::string& firstName)
{
    return !firstName.empty() && firstName.length() <= 50;
}

bool isValidLastName(const std::string& lastName)
{
    return !lastName.empty() && lastName.length() <= 50;
}

bool isValidEmail(const std::string& email)
{
    if(email.empty() || email.length() > 100) return false;

    auto atPos = email.find('@');
    auto dotPos = email.rfind('.');

    // Mora postojati @ i tačka nakon njega
    if(atPos == std::string::npos || dotPos == std::string::npos) return false;

    if(atPos == 0 || atPos == email.length() - 1) return false; // @ ne sme biti na početku/ kraju
    if(dotPos < atPos || dotPos == email.length() - 1) return false; // tačka mora biti nakon @ i ne poslednja

    return true;
}


bool isValidPin(int pin)
{
    return pin >= 1000 && pin <= 9999;  // 4-digitni PIN
}