#include "../../../include/common/models/ServiceReport.hpp"
#include <stdexcept>
#include <limits>

ServiceReport::ServiceReport() : id(-1), workOrderID(-1) {}

ServiceReport::ServiceReport(int id, int workOrderID) : id(id), workOrderID(workOrderID) {}

// Getters
int ServiceReport::getId() const
{
    return id;
}
int ServiceReport::getWorkOrderID() const
{
    return workOrderID;
}

// Setters with validation
void ServiceReport::setId(int id)
{
    if (!isValidId(id))
        throw std::invalid_argument("ID servisnog izvestaja mora biti izmedju 0 i 2147483646");
    this->id = id;
}
void ServiceReport::setWorkOrderID(int workOrderID)
{
    if (!isValidId(workOrderID))
        throw std::invalid_argument("ID radnog naloga mora biti izmedju 0 i 2147483646");
    this->workOrderID = workOrderID;
}

// Validation functions
bool isValidId(int id)
{
    return id >= 0 && id < INT_MAX;
}
