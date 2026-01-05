#include "../../../include/common/util/Validate.hpp"

#include "../../../include/common/models/Bill.hpp"
#include "../../../include/common/models/Device.hpp"
#include "../../../include/common/models/WorkOrder.hpp"
#include "../../../include/common/models/Worker.hpp"
#include "../../../include/common/util/Validate.hpp"

namespace Validate {
bool isValidId (int id) {
    return id >= 0 && id < INT_MAX;
}
bool isValidPaymentMethod (PaymentMethod paymentMethod) {
    return paymentMethod == PaymentMethod::CASH || paymentMethod == PaymentMethod::CREDIT_CARD ||
           paymentMethod == PaymentMethod::DIGITAL_WALLET;
}
bool isValidPrice (double price) {
    return price >= 0.0;
}
bool isValidNonNegative (int amount) {
    return amount >= 0.0;
}

bool isValidBrand (const std::string& brand) {
    return !brand.empty () && brand.length () <= 50;
}
bool isValidModel (const std::string& model) {
    return !model.empty () && model.length () <= 50;
}
bool isValidIMEI (const std::string& deviceIMEI) {
    /* Ovdi bi trebala da ide implementacija Luhnovog algoritma, ali pošto nemamo vremena da se bakćemo copypasteom IMEI kodova nema
     * ništa od toga, yippie!*/
    if (deviceIMEI.length () != 15) return false;
    for (char c : deviceIMEI) {
        if (!std::isdigit (static_cast<unsigned char> (c))) return false;
    }
    return true;
}
bool isValidState (State state) {
    if (state == State::NEW || state == State::USED) return true;
    return false;
}
bool isValidPhone (const std::string& phone) {
    if (phone.empty () || phone.length () > 15) return false;

    for (char c : phone) {
        if (!isdigit (c) && c != '+' && c != '-' && c != ' ') return false;
    }
    return true;
}
bool isValidPin (int pin) {
    return pin >= 1000 && pin <= 9999;
}

bool isValidFirstName (const std::string& firstName) {
    return !firstName.empty () && firstName.length () <= 50;
}
bool isValidLastName (const std::string& lastName) {
    return !lastName.empty () && lastName.length () <= 50;
}
bool isValidEmail (const std::string& email) {
    if (email.empty () || email.length () > 100) return false;

    auto atPos = email.find ('@');
    auto dotPos = email.rfind ('.');

    if (atPos == std::string::npos || dotPos == std::string::npos) return false;

    if (atPos == 0 || atPos == email.length () - 1) return false;
    if (dotPos == email.length () - 1) return false;

    if (dotPos == atPos + 1) return false;

    return true;
}

bool isValidPartName (const std::string& name) {
    return !name.empty () && name.length () <= 100;
}
bool isValidDescription (const std::string& description) {
    return !description.empty () && description.length () <= 200;
}
bool isValidPriceAssessment (double priceAssessment) {
    return priceAssessment >= 0.0;
}

bool isValidRole (Role role) {
    if (role == Role::RECEPTIONIST || role == Role::TECHNICIAN || role == Role::SERVICE_OWNER) return true;
    return false;
}

bool isValidStatus (WorkOrderStatus status) {
    return status == WorkOrderStatus::IN_DIAGNOSTICS || status == WorkOrderStatus::IN_REPAIR ||
           status == WorkOrderStatus::WAITING_FOR_PARTS || status == WorkOrderStatus::COMPLETED;
}
bool isValidComment (const std::string& comment) {
    return comment.length () <= 100;
}
bool isValidStartDate (std::time_t startDate) {
    std::time_t now = std::time (nullptr);
    return startDate <= now;
}
bool isValidEndDate (std::time_t startDate, std::time_t endDate) {
    std::time_t now = std::time (nullptr);
    return endDate >= startDate && endDate <= now;
}
bool isValidUsedParts (const std::map<int, int>& usedParts) {
    for (const auto& part : usedParts) {
        if (part.first <= 0 || part.second <= 0) return false;
    }
    return true;
}

}  // namespace Validate