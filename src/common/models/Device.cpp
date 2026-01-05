#include "../../../include/common/models/Device.hpp"
#include <algorithm>
#include <limits>
#include <stdexcept>

Device::Device()
    : id(-1), brand(""), model(""), IMEI(""), state(State::UNKNOWN)
{
}
Device::Device(int id, const std::string &brand, const std::string &model,
               const std::string &IMEI, State state)
    : id(id), brand(brand), model(model), IMEI(IMEI), state(state)
{
}

int Device::getId() const { return id; }
const std::string &Device::getBrand() const { return brand; }
const std::string &Device::getModel() const { return model; }
const std::string &Device::getIMEI() const { return IMEI; }
State Device::getState() const { return state; }

void Device::setId(int v)
{
    if (!isValidId(v))
        throw std::invalid_argument("ID uredjaja mora biti izmedju 0 i 2147483646");
    id = v;
}
void Device::setBrand(const std::string &v)
{
    if (!isValidBrand(v))
        throw std::invalid_argument("Naziv brenda mora biti neprazan i ne sme biti duzi od 50 karaktera");
    brand = v;
}
void Device::setModel(const std::string &v)
{
    if (!isValidModel(v))
        throw std::invalid_argument("Naziv modela mora biti neprazan i ne sme biti duzi od 50 karaktera");
    model = v;
}
void Device::setIMEI(const std::string &v)
{
    if (!isValidIMEI(v))
        throw std::invalid_argument("IMEI mora imati tacno 15 cifara");
    IMEI = v;
}
void Device::setState(State v)
{
    if (!isValidState(v))
        throw std::invalid_argument("Nevalidno stanje");
    state = v;
}

bool isValidId(int id)
{
    return id >= 0 && id < INT_MAX;
}
bool isValidBrand(const std::string &brand)
{
    return !brand.empty() && brand.length() <= 50;
}
bool isValidModel(const std::string &model)
{
    return !model.empty() && model.length() <= 50;
}
bool isValidIMEI(const std::string &deviceIMEI)
{
    /* Ovdi bi trebala da ide implementacija Luhnovog algoritma, ali pošto nemamo vremena da se bakćemo copypasteom IMEI kodova nema ništa od toga, yippie!*/
    return deviceIMEI.length() == 15;
}
bool isValidState(State state)
{
    if (state == State::NEW ||
        state == State::USED)
        return true;
    return false;
}