#include "../../../include/common/models/Device.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>

#include "../../../include/common/util/Validate.hpp"
Device::Device () : id (-1), brand (""), model (""), IMEI (""), state (State::UNKNOWN) {}
Device::Device (int id, const std::string& brand, const std::string& model, const std::string& IMEI, State state)
    : id (id), brand (brand), model (model), IMEI (IMEI), state (state) {}

int Device::getId () const {
    return id;
}
const std::string& Device::getBrand () const {
    return brand;
}
const std::string& Device::getModel () const {
    return model;
}
const std::string& Device::getIMEI () const {
    return IMEI;
}
State Device::getState () const {
    return state;
}

void Device::setId (int v) {
    if (!Validate::isValidId (v)) throw std::invalid_argument ("ID uredjaja mora biti izmedju 0 i 2147483646");
    id = v;
}
void Device::setBrand (const std::string& v) {
    if (!Validate::isValidBrand (v))
        throw std::invalid_argument ("Naziv brenda mora biti neprazan i ne sme biti duzi od 50 karaktera");
    brand = v;
}
void Device::setModel (const std::string& v) {
    if (!Validate::isValidModel (v))
        throw std::invalid_argument ("Naziv modela mora biti neprazan i ne sme biti duzi od 50 karaktera");
    model = v;
}
void Device::setIMEI (const std::string& v) {
    if (!Validate::isValidIMEI (v)) throw std::invalid_argument ("IMEI mora imati tacno 15 cifara");
    IMEI = v;
}
void Device::setState (State v) {
    if (!Validate::isValidState (v)) throw std::invalid_argument ("Nevalidno stanje");
    state = v;
}
