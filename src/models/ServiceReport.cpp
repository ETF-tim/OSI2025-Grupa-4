#include "../../include/models/ServiceReport.hpp"
#include <stdexcept>
#include <limits>

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
        throw std::invalid_argument("Service Report ID must be between 0 and 2147483647");
    this->id = id;
}
void ServiceReport::setWorkOrderID(int workOrderID)
{
    if (!isValidWorkOrderID(workOrderID))
        throw std::invalid_argument("Work Order ID must be between 0 and 2147483647");
    this->workOrderID = workOrderID;
}

// Validation functions
bool isValidId(int id)
{
    return id > 0 && id < INT_MAX;
}
bool isValidWorkOrderID(int workOrderID)
{
    return workOrderID > 0 && workOrderID < INT_MAX;
}