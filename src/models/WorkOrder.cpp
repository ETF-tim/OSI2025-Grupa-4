#include "../../include/models/WorkOrder.hpp"
#include <stdexcept>
#include <limits>

WorkOrder::WorkOrder()
    : id(0), receiptOrderID(0), status(WorkOrderStatus::IN_DIAGNOSTICS), comment(), startDate(0), endDate(0), technicianID(0), usedParts(), servicePrice(0.0)
{
}

WorkOrder::WorkOrder(int id, int receiptOrderID, WorkOrderStatus status, const std::string &comment,
                     std::time_t startDate, std::time_t endDate, int technicianID, double servicePrice)
    : id(id), receiptOrderID(receiptOrderID), status(status), comment(comment), startDate(startDate), endDate(endDate), technicianID(technicianID), usedParts(), servicePrice(servicePrice)
{
}

// Getters
int WorkOrder::getId() const { return id; }
int WorkOrder::getReceiptOrderID() const { return receiptOrderID; }
WorkOrderStatus WorkOrder::getStatus() const { return status; }
std::string WorkOrder::getComment() const { return comment; }
std::time_t WorkOrder::getStartDate() const { return startDate; }
std::time_t WorkOrder::getEndDate() const { return endDate; }
int WorkOrder::getTechnicianID() const { return technicianID; }
std::map<int, int> WorkOrder::getUsedParts() const { return usedParts; }
double WorkOrder::getServicePrice() const { return servicePrice; }

// Setters
void WorkOrder::setId(int v)
{
    if (!isValidId(v))
        throw std::invalid_argument("WorkOrderID must be between 0 and 2147483647");
    id = v;
}
void WorkOrder::setReceiptOrderID(int v)
{
    if (!isValidReceiptOrderID(v))
        throw std::invalid_argument("ReceiptOrderID must be between 0 and 2147483647");
    receiptOrderID = v;
}
void WorkOrder::setStatus(WorkOrderStatus v)
{
    if (!isValidStatus(v))
        throw std::invalid_argument("Invalid WorkOrder status");
    status = v;
}
void WorkOrder::setComment(const std::string &v)
{
    if (!isValidComment(v))
        throw std::invalid_argument("Comment must be shorter than 100 characters");
    comment = v;
}
void WorkOrder::setStartDate(std::time_t v)
{
    if (!isValidStartDate(v))
        throw std::invalid_argument("Invalid start date");
    startDate = v;
}
void WorkOrder::setEndDate(std::time_t v)
{
    if (!isValidEndDate(startDate, v))
        throw std::invalid_argument("Invalid end date");
    endDate = v;
}
void WorkOrder::setTechnicianID(int v)
{
    if (!isValidTechnicianID(v))
        throw std::invalid_argument("TechnicianID must be between 0 and 2147483647");
    technicianID = v;
}
void WorkOrder::setUsedParts(const std::map<int, int> &v)
{
    if (!isValidUsedParts(v))
        throw std::invalid_argument("UsedParts must have positive part IDs and quantities");
    usedParts = v;
}
void WorkOrder::setServicePrice(double v)
{
    if (!isValidServicePrice(v))
        throw std::invalid_argument("ServicePrice must be non-negative");
    servicePrice = v;
}

// Validation functions
bool isValidId(int id)
{
    return id > 0 && id < INT_MAX;
}
bool isValidReceiptOrderID(int receiptOrderID)
{
    return receiptOrderID > 0 && receiptOrderID < INT_MAX;
}
bool isValidStatus(WorkOrderStatus status)
{
    return status == WorkOrderStatus::IN_DIAGNOSTICS ||
           status == WorkOrderStatus::IN_REPAIR ||
           status == WorkOrderStatus::WAITING_FOR_PARTS ||
           status == WorkOrderStatus::COMPLETED;
}
bool isValidComment(const std::string &comment)
{
    return comment.length() <= 100; // Example: comment should not exceed 100 characters
}
bool isValidStartDate(std::time_t startDate)
{
    std::time_t now = std::time(nullptr);
    return startDate <= now; // Start date should not be in the future
}
bool isValidEndDate(std::time_t startDate, std::time_t endDate)
{
    return endDate >= startDate; // End date should not be before start date
}
bool isValidTechnicianID(int technicianID)
{
    return technicianID > 0 && technicianID < INT_MAX;
}
bool isValidUsedParts(const std::map<int, int> &usedParts)
{
    for (const auto &part : usedParts)
    {
        if (part.first <= 0 || part.second <= 0)
            return false; // Part ID and quantity must be positive
    }
    return true;
}
bool isValidServicePrice(double servicePrice)
{
    return servicePrice >= 0.0;
}