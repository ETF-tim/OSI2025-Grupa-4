#include "../../../include/common/models/Person.hpp"
#include "../../../include/common/util/Validate.hpp"
#include <limits>
#include <stdexcept>

Person::Person () : id (-1), firstname (""), lastname (""), email (""), phone (""), pin (-1) {}
Person::Person (int id, const std::string& firstname, const std::string& lastname, const std::string& email, const std::string& phone,
                int pin)
    : id (id), firstname (firstname), lastname (lastname), email (email), phone (phone), pin (pin) {}

int Person::getId () const {
    return id;
}
const std::string& Person::getFirstName () const {
    return firstname;
}
const std::string& Person::getLastName () const {
    return lastname;
}
const std::string& Person::getPhone () const {
    return phone;
}
const std::string& Person::getEmail () const {
    return email;
}
int Person::getPin () const {
    return pin;
}

void Person::setId (int v) {
    if (!Validate::isValidId (v)) throw std::invalid_argument ("ID osobe mora biti između 0 i 2147483646");
    id = v;
}
void Person::setFirstName (const std::string& v) {
    if (!Validate::isValidFirstName (v)) throw std::invalid_argument ("Ime ne smije biti prazno i mora imati manje od 50 karaktera");
    firstname = v;
}
void Person::setLastName (const std::string& v) {
    if (!Validate::isValidLastName (v))
        throw std::invalid_argument ("Prezime ne smije biti prazno i mora imati manje od 50 karaktera");
    lastname = v;
}
void Person::setEmail (const std::string& v) {
    if (!Validate::isValidEmail (v)) throw std::invalid_argument ("Neispravan e-mail");
    email = v;
}
void Person::setPhone (const std::string& v) {
    if (!Validate::isValidPhone (v)) throw std::invalid_argument ("Neispravan broj telefona");
    phone = v;
}
void Person::setPin (int v) {
    if (!Validate::isValidPin (v)) throw std::invalid_argument ("Neispravan PIN");
    pin = v;
}


