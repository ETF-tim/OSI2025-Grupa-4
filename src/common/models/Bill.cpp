#include "../../../include/common/models/Bill.hpp"

#include <limits>
#include <stdexcept>

#include "../../../include/common/util/Validate.hpp"
Bill::Bill () : id (-1), workOrderID (-1), paymentMethod (PaymentMethod::UNKNOWN), price (-1) {};

Bill::Bill (int id, int workOrderID, PaymentMethod paymentMethod, double price) {
    Bill::setId (id);
    Bill::setWorkOrderID (workOrderID);
    Bill::setPaymentMethod (paymentMethod);
    Bill::setPrice (price);
}

// Getters
int Bill::getId () const {
    return id;
}

int Bill::getWorkOrderID () const {
    return workOrderID;
}

PaymentMethod Bill::getPaymentMethod () const {
    return paymentMethod;
}

double Bill::getPrice () const {
    return price;
}

// Setters with validation
void Bill::setId (int id) {
    if (!Validate::isValidId (id)) throw std::invalid_argument ("ID racuna mora biti izmedju 0 i 2147483646");
    this->id = id;
}

void Bill::setWorkOrderID (int workOrderID) {
    if (!Validate::isValidId (workOrderID)) throw std::invalid_argument ("ID radnog naloga mora biti izmedju 0 i 2147483646");
    this->workOrderID = workOrderID;
}

void Bill::setPaymentMethod (PaymentMethod paymentMethod) {
    if (!Validate::isValidPaymentMethod (paymentMethod)) throw std::invalid_argument ("Neispravan nacin placanja");
    this->paymentMethod = paymentMethod;
}

void Bill::setPrice (double price) {
    if (!Validate::isValidPrice (price)) throw std::invalid_argument ("Cijena mora biti nenegativna");
    this->price = price;
}
