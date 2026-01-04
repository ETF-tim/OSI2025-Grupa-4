#include "../../include/models/ServiceReport.hpp"
#include <stdexcept>
#include <limits>

int ServiceReport::getId() const
{
    return id;
}
int ServiceReport::getWorkOrderID() const
{
    return workOrderID;
}

void ServiceReport::setId(int id)
{
    if (!isValidId(id))
        throw std::invalid_argument("ID servisnog izvestaja mora biti izmedju 0 i 2147483646");
    this->id = id;
}
void ServiceReport::setWorkOrderID(int workOrderID)
{
    if (!isValidWorkOrderID(workOrderID))
        throw std::invalid_argument("ID radnog naloga mora biti izmedju 0 i 2147483646");
    this->workOrderID = workOrderID;
}

bool isValidId(int id)
{
    return id > 0 && id < INT_MAX;
}
bool isValidWorkOrderID(int workOrderID)
{
    return workOrderID > 0 && workOrderID < INT_MAX;
}