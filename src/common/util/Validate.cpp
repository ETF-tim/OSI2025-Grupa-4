#include "../../../include/common/util/Validate.hpp"

#include <algorithm>
#include <iostream>

#include "../../../include/common/models/Bill.hpp"
#include "../../../include/common/models/Device.hpp"
#include "../../../include/common/models/WorkOrder.hpp"
#include "../../../include/common/models/Worker.hpp"
#include "../../../include/common/util/Validate.hpp"

namespace Validate {
// ---Common---
bool isValidId (int id) {
    if (id >= 0 && id < INT_MAX)
        return true;
    else {
        std::cerr << "ID mora biti izmedju 0 i 2147483646" << std::endl;
        return false;
    }
}
bool isValidNonNegative (int amount) {
    if (amount >= 0) {
        return true;
    } else {
        std::cerr << "Vrijednost mora biti nenegativna" << std::endl;
        return false;
    }
}

// -- Service data ---
bool isValidServiceName (const std::string& name) {
    return !name.empty () && name.length () <= 20;
}
bool isValidServiceAddress (const std::string& address) {
    return !address.empty () && address.length () <= 50;
}
bool isValidJIB (const std::string& jib) {
    return jib.length () == 13 && std::all_of (jib.begin (), jib.end (), ::isdigit);
}

// Bill
bool isValidPaymentMethod (PaymentMethod paymentMethod) {
    if (paymentMethod == PaymentMethod::CASH || paymentMethod == PaymentMethod::CREDIT_CARD ||
        paymentMethod == PaymentMethod::DIGITAL_WALLET) {
        return true;
    } else {
        std::cerr << "Neispravan nacin placanja" << std::endl;
        return false;
    }
}
bool isValidPrice (double price) {
    if (price >= 0.0) {
        return true;
    } else {
        std::cerr << "Cijena mora biti nenegativna" << std::endl;
        return false;
    }
}

// Device
bool isValidBrand (const std::string& brand) {
    if (!brand.empty () && brand.length () <= 50) {
        return true;
    } else {
        std::cerr << "Naziv brenda mora biti neprazan i ne sme biti duzi od 50 karaktera" << std::endl;
        return false;
    }
}
bool isValidModel (const std::string& model) {
    if (!model.empty () && model.length () <= 50) {
        return true;
    } else {
        std::cerr << "Naziv modela mora biti neprazan i ne sme biti duzi od 50 karaktera" << std::endl;
        return false;
    }
}
bool isValidIMEI (const std::string& deviceIMEI) {
    /* Ovdi bi trebala da ide implementacija Luhnovog algoritma, ali pošto nemamo vremena da se bakćemo copypasteom IMEI kodova nema
     * ništa od toga, yippie!*/
    if (deviceIMEI.length () != 15) {
        std::cerr << "IMEI nije validan" << std::endl;
        return false;
    }
    for (char c : deviceIMEI) {
        if (!std::isdigit (static_cast<unsigned char> (c))) {
            std::cerr << "IMEI mora imati tacno 15 cifara" << std::endl;
            return false;
        }
    }
    return true;
}
bool isValidState (State state) {
    if (state == State::NEW || state == State::USED)
        return true;
    else {
        std::cerr << "Nevalidno stanje" << std::endl;
        return false;
    }
}

// Part
bool isValidPartName (const std::string& name) {
    if (!name.empty () && name.length () <= 100)
        return true;
    else {
        std::cerr << "Naziv dijela ne smije biti prazan i mora imati najvise 100 karaktera" << std::endl;
        return false;
    }
}

// Person
bool isValidFirstName (const std::string& firstName) {
    if (!firstName.empty () && firstName.length () <= 50)
        return true;
    else {
        std::cerr << "Ime ne smije biti prazno i mora imati manje od 50 karaktera" << std::endl;
        return false;
    }
}
bool isValidLastName (const std::string& lastName) {
    if (!lastName.empty () && lastName.length () <= 50)
        return true;
    else {
        std::cerr << "Prezime ne smije biti prazno i mora imati manje od 50 karaktera" << std::endl;
        return false;
    }
}
bool isValidEmail (const std::string& email) {
    if (email.empty () || email.length () > 100) {
        std::cerr << "Neispravan e-mail" << std::endl;
        return false;
    }

    auto atPos = email.find ('@');
    auto dotPos = email.rfind ('.');

    if (atPos == std::string::npos || dotPos == std::string::npos) {
        std::cerr << "Neispravan e-mail" << std::endl;
        return false;
    }

    if (atPos == 0 || atPos == email.length () - 1) {
        std::cerr << "Neispravan e-mail" << std::endl;
        return false;
    }
    if (dotPos == email.length () - 1) {
        std::cerr << "Neispravan e-mail" << std::endl;
        return false;
    }

    if (dotPos == atPos + 1) {
        std::cerr << "Neispravan e-mail" << std::endl;
        return false;
    }
    return true;
}
bool isValidPhone (const std::string& phone) {
    if (phone.empty () || phone.length () > 15) {
        std::cerr << "Neispravan broj telefona" << std::endl;
        return false;
    }
    for (char c : phone) {
        if (!isdigit (c) && c != '+' && c != '-' && c != ' ') {
            std::cerr << "Neispravan broj telefona" << std::endl;
            return false;
        }
    }
    return true;
}
bool isValidPin (int pin) {
    if (pin >= 1000 && pin <= 9999)
        return true;
    else {
        std::cerr << "Neispravan PIN" << std::endl;
        return false;
    }
}

// Receipt order
bool isValidDescription (const std::string& description) {
    if (!description.empty () && description.length () <= 200)
        return true;
    else {
        std::cerr << "Opis mora biti duzine do 200 karaktera" << std::endl;
        return false;
    }
}
bool isValidPriceAssessment (double priceAssessment) {
    if (priceAssessment >= 0.0)
        return true;
    else {
        std::cerr << "Procjena cijene mora biti nenegativna" << std::endl;
        return false;
    }
}

// Worker
bool isValidRole (Role role) {
    if (role == Role::RECEPTIONIST || role == Role::TECHNICIAN || role == Role::SERVICE_OWNER)
        return true;
    else {
        std::cerr << "Neispravna uloga radnika" << std::endl;
        return false;
    }
}

// Work order
bool isValidStatus (WorkOrderStatus status) {
    if (status == WorkOrderStatus::IN_DIAGNOSTICS || status == WorkOrderStatus::IN_REPAIR ||
        status == WorkOrderStatus::WAITING_FOR_PARTS || status == WorkOrderStatus::COMPLETED)
        return true;
    else {
        std::cerr << "Neispravan status popravke" << std::endl;
        return false;
    }
}
bool isValidComment (const std::string& comment) {
    if (comment.length () <= 100)
        return true;
    else {
        std::cerr << "Komentar mora biti kraci od 100 karaktera" << std::endl;
        return false;
    }
}
bool isValidStartDate (std::time_t startDate) {
    std::time_t now = std::time (nullptr);
    if (startDate <= now)
        return true;
    else {
        std::cerr << "Neispravan datum pocetka popravke" << std::endl;
        return false;
    }
}
bool isValidEndDate (std::time_t startDate, std::time_t endDate) {
    std::time_t now = std::time (nullptr);
    if (endDate >= startDate && endDate <= now)
        return true;
    else {
        std::cerr << "Neispravan datum zavrsetka popravke" << std::endl;
        return false;
    }
}
bool isValidUsedParts (const std::map<int, int>& usedParts) {
    for (const auto& part : usedParts) {
        if (part.first <= 0 || part.second <= 0) {
            std::cerr << "ID dijela i kolicina moraju biti pozitivni" << std::endl;
            return false;
        }
    }
    return true;
}

}  // namespace Validate