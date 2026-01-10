#include "../../../include/common/models/Receipt_order.hpp"

#include <limits>
#include <stdexcept>

#include "../../../include/common/util/Validate.hpp"

ReceiptOrder::ReceiptOrder () : id (-1), userID (-1), deviceIMEI (""), description (""), priceAssessment (-1), isFree (true) {}

ReceiptOrder::ReceiptOrder (int id, int userID, const std::string& deviceIMEI, const std::string& description, double priceAssessment)
    : isFree (true) {
    ReceiptOrder::setId (id);
    ReceiptOrder::setUserId (userID);
    ReceiptOrder::setDeviceIMEI (deviceIMEI);
    ReceiptOrder::setDescription (description);
    ReceiptOrder::setPriceAssessment (priceAssessment);
}

int ReceiptOrder::getId () const {
    return id;
}
int ReceiptOrder::getUserId () const {
    return userID;
}
const std::string& ReceiptOrder::getDeviceIMEI () const {
    return deviceIMEI;
}
const std::string& ReceiptOrder::getDescription () const {
    return description;
}
double ReceiptOrder::getPriceAssessment () const {
    return priceAssessment;
}
bool ReceiptOrder::getIsFree () const {
    return isFree;
}

void ReceiptOrder::setId (int v) {
    if (!Validate::isValidId (v)) {
        throw std::invalid_argument ("ID prijemnog naloga mora biti izmedju 0 i 2147483646");
    }
    id = v;
}

void ReceiptOrder::setUserId (int v) {
    if (!Validate::isValidId (v)) {
        throw std::invalid_argument ("ID korisnickog naloga mora biti izmedju 0 i 2147483646");
    }
    userID = v;
}

void ReceiptOrder::setDeviceIMEI (const std::string& v) {
    if (!Validate::isValidIMEI (v)) {
        throw std::invalid_argument ("IMEI uredjaja mora biti duzine 15 karaktera");
    }
    deviceIMEI = v;
}

void ReceiptOrder::setDescription (const std::string& v) {
    if (!Validate::isValidDescription (v)) {
        throw std::invalid_argument ("Opis mora biti duzine do 200 karaktera");
    }
    description = v;
}

void ReceiptOrder::setPriceAssessment (double v) {
    if (!Validate::isValidPriceAssessment (v)) {
        throw std::invalid_argument ("Procjena cijene mora biti nenegativna");
    }
    priceAssessment = v;
}

void ReceiptOrder::setIsFree (bool v) {
    isFree = v;
}
