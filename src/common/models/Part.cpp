#include "../../../include/common/models/Part.hpp"
#include <stdexcept>
#include <limits>

Part::Part() : id(-1), name(""), amount(-1), criticalAmount(-1), price(-1) {}
Part::Part(int id, const std::string &name, int amount, int criticalAmount, double price)
    : id(id), name(name), amount(amount), criticalAmount(criticalAmount), price(price) {}

// Getters
int Part::getId() const
{
    return id;
}
const std::string &Part::getName() const
{
    return name;
}
int Part::getAmount() const
{
    return amount;
}
int Part::getCriticalAmount() const
{
    return criticalAmount;
}
double Part::getPrice() const
{
    return price;
}

// Setters with validation
void Part::setId(int v)
{
    if (!isValidId(v))
        throw std::invalid_argument("ID dijela mora biti izmedju 0 i 2147483646");
    id = v;
}
void Part::setName(const std::string &v)
{
    if (!isValidName(v))
        throw std::invalid_argument("Naziv dijela ne smije biti prazan i mora imati najvise 100 karaktera");

    name = v;
}
void Part::setAmount(int v)
{
    if (!isValidAmount(v))
        throw std::invalid_argument("Kolicina mora biti nenegativna");

    amount = v;
}
void Part::setCriticalAmount(int v)
{
    if (!isValidCriticalAmount(v))
        throw std::invalid_argument("Kriticna kolicina mora biti nenegativna");

    criticalAmount = v;
}
void Part::setPrice(double v)
{
    if (!isValidPrice(v))
        throw std::invalid_argument("Cijena mora biti nenegativna");

    price = v;
}

// Validation functions
bool isValidId(int id)
{
    return id >= 0 && id < INT_MAX;
}

bool isValidName(const std::string &name)
{
    return !name.empty() && name.length() <= 100;
}

bool isValidAmount(int amount)
{
    return amount >= 0; // Količina mora biti nenegativna
}

bool isValidCriticalAmount(int criticalAmount)
{
    return criticalAmount >= 0; // Kritična količina mora biti nenegativna
}

bool isValidPrice(double price)
{
    return price >= 0.0; // Cijena mora biti nenegativna
}