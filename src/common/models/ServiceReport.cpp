#include "../../../include/common/models/ServiceReport.hpp"

#include <limits>
#include <stdexcept>

#include "../../../include/common/util/Validate.hpp"

ServiceReport::ServiceReport () : id (-1), workOrderID (-1) {}

ServiceReport::ServiceReport (int id, int workOrderID) : id (id), workOrderID (workOrderID) {}

// Getters
int ServiceReport::getId () const {
    return id;
}
int ServiceReport::getWorkOrderID () const {
    return workOrderID;
}

// Setters with validation
void ServiceReport::setId (int id) {
    if (!Validate::isValidId (id)) throw std::invalid_argument ("ID servisnog izvestaja mora biti izmedju 0 i 2147483646");
    this->id = id;
}
void ServiceReport::setWorkOrderID (int workOrderID) {
    if (!Validate::isValidId (workOrderID)) throw std::invalid_argument ("ID radnog naloga mora biti izmedju 0 i 2147483646");
    this->workOrderID = workOrderID;
}
