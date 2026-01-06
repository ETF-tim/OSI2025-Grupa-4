#include "../../../include/common/models/Worker.hpp"

#include <limits>
#include <stdexcept>

#include "../../../include/common/util/Validate.hpp"

Worker::Worker () : Person (), role (Role::UNKNOWN) {}
Worker::Worker (int id, const std::string& firstname, const std::string& lastname, const std::string& email, const std::string& phone,
                int pin, Role role)
    : Person (id, firstname, lastname, email, phone, pin), role (role) {}

Role Worker::getRole () const {
    return role;
}

void Worker::setRole (Role v) {
    if (!Validate::isValidRole (v)) throw std::invalid_argument ("Neispravna uloga radnika");
    role = v;
}
