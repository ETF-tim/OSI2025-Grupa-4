#include "../../../include/common/models/Person.hpp"
#include <stdexcept>
#include <limits>

Person::Person()
    : id(-1), firstname(""), lastname(""), email(""), phone(""), pin(-1) {}
Person::Person(int id, const std::string &firstname, const std::string &lastname, const std::string &email, const std::string &phone, int pin)
    : id(id), firstname(firstname), lastname(lastname), email(email), phone(phone), pin(pin) {}

// Getters
int Person::getId() const
{
    return id;
}
const std::string &Person::getFirstName() const
{
    return firstname;
}
const std::string &Person::getLastName() const
{
    return lastname;
}
const std::string &Person::getPhone() const
{
    return phone;
}
const std::string &Person::getEmail() const
{
    return email;
}
int Person::getPin() const noexcept
{
    return pin;
}

// Setters with validation
void Person::setId(int v)
{
    if (!isValidId(v))
        throw std::invalid_argument("ID osobe mora biti između 0 i 2147483646");
    id = v;
}
void Person::setFirstName(const std::string &v)
{
    if (!isValidFirstName(v))
        throw std::invalid_argument("Ime ne smije biti prazno i mora imati manje od 50 karaktera");
    firstname = v;
}
void Person::setLastName(const std::string &v)
{
    if (!isValidLastName(v))
        throw std::invalid_argument("Prezime ne smije biti prazno i mora imati manje od 50 karaktera");
    lastname = v;
}
void Person::setEmail(const std::string &v)
{
    if (!isValidEmail(v))
        throw std::invalid_argument("Neispravan e-mail");
    email = v;
}
void Person::setPhone(const std::string &v)
{
    if (!isValidPhone(v))
        throw std::invalid_argument("Neispravan broj telefona");
    phone = v;
}
void Person::setPin(int v)
{
    if (!isValidPin(v))
        throw std::invalid_argument("Neispravan PIN");
    pin = v;
}

// Validation functions
bool isValidId(int id)
{
    return id >= 0 && id < INT_MAX;
}
bool isValidFirstName(const std::string &firstName)
{
    return !firstName.empty() && firstName.length() <= 50;
}
bool isValidLastName(const std::string &lastName)
{
    return !lastName.empty() && lastName.length() <= 50;
}
bool isValidEmail(const std::string &email)
{
    if (email.empty() || email.length() > 100)
        return false;

    auto atPos = email.find('@');
    auto dotPos = email.rfind('.');

    // Mora postojati @ i tačka nakon njega
    if (atPos == std::string::npos || dotPos == std::string::npos)
        return false;

    if (atPos == 0 || atPos == email.length() - 1)
        return false; // @ ne sme biti na početku/ kraju
    if (dotPos < atPos || dotPos == email.length() - 1)
        return false; // tačka mora biti nakon @ i ne poslednja

    return true;
}
bool isValidPhone(const std::string &phone)
{
    if (phone.empty() || phone.length() > 15)
        return false;

    for (char c : phone)
    {
        if (!isdigit(c) && c != '+' && c != '-' && c != ' ')
            return false;
    }
    return true;
}
bool isValidPin(int pin)
{
    return pin >= 1000 && pin <= 9999; // 4-cifreni PIN
}