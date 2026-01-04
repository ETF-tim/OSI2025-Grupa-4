#include "../../include/models/Bill.hpp"
#include <stdexcept>
#include <limits>

// Getters
int Bill::getId() const
{
    return id;
}

int Bill::getReceiptOrderID() const
{
    return receiptOrderID;
}

PaymentMethod Bill::getPaymentMethod() const
{
    return paymentMethod;
}

double Bill::getPrice() const
{
    return price;
}

// Setters with validation
void Bill::setId(int id)
{
    if (!isValidId(id))
        throw std::invalid_argument("ID racuna mora biti izmedju 0 i 2147483646");
    this->id = id;
}

void Bill::setReceiptOrderID(int receiptOrderID)
{
    if (!isValidReceiptOrderID(receiptOrderID))
        throw std::invalid_argument("ID prijemnog naloga mora biti izmedju 0 i 2147483646");
    this->receiptOrderID = receiptOrderID;
}

void Bill::setPaymentMethod(PaymentMethod paymentMethod)
{
    if (!isValidPaymentMethod(paymentMethod))
        throw std::invalid_argument("Neispravan nacin placanja");
    this->paymentMethod = paymentMethod;
}

void Bill::setPrice(double price)
{
    if (!isValidPrice(price))
        throw std::invalid_argument("Cijena mora biti nenegativna");
    this->price = price;
}

// Validation functions
bool isValidId(int id)
{
    return id > 0 && id < INT_MAX;
}

bool isValidReceiptOrderID(int receiptOrderID)
{
    return receiptOrderID > 0 && receiptOrderID < INT_MAX;
}

bool isValidPaymentMethod(PaymentMethod paymentMethod)
{
    return paymentMethod == PaymentMethod::CASH ||
           paymentMethod == PaymentMethod::CREDIT_CARD ||
           paymentMethod == PaymentMethod::DIGITAL_WALLET;
}

bool isValidPrice(double price)
{
    return price >= 0.0;
}
