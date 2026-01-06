#include "../../../include/common/models/Part.hpp"

#include <limits>
#include <stdexcept>

#include "../../../include/common/util/Validate.hpp"
Part::Part () : id (-1), name (""), amount (-1), criticalAmount (-1), price (-1) {}
Part::Part (int id, const std::string& name, int amount, int criticalAmount, double price)
    : id (id), name (name), amount (amount), criticalAmount (criticalAmount), price (price) {}

// Getters
int Part::getId () const {
    return id;
}
const std::string& Part::getName () const {
    return name;
}
int Part::getAmount () const {
    return amount;
}
int Part::getCriticalAmount () const {
    return criticalAmount;
}
double Part::getPrice () const {
    return price;
}

void Part::setId (int v) {
    if (!Validate::isValidId (v)) throw std::invalid_argument ("ID dijela mora biti izmedju 0 i 2147483646");
    id = v;
}
void Part::setName (const std::string& v) {
    if (!Validate::isValidPartName (v))
        throw std::invalid_argument ("Naziv dijela ne smije biti prazan i mora imati najvise 100 karaktera");

    name = v;
}
void Part::setAmount (int v) {
    if (!Validate::isValidNonNegative (v)) throw std::invalid_argument ("Kolicina mora biti nenegativna");

    amount = v;
}
void Part::setCriticalAmount (int v) {
    if (!Validate::isValidNonNegative (v)) throw std::invalid_argument ("Kriticna kolicina mora biti nenegativna");

    criticalAmount = v;
}
void Part::setPrice (double v) {
    if (!Validate::isValidNonNegative (v)) throw std::invalid_argument ("Cijena mora biti nenegativna");

    price = v;
}
