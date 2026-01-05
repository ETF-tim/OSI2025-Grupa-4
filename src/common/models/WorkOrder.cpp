#include "../../../include/common/models/WorkOrder.hpp"

#include <limits>
#include <stdexcept>

WorkOrder::WorkOrder (int id, int receiptOrderID, WorkOrderStatus status, const std::string& comment, std::time_t startDate,
                      std::time_t endDate, int technicianID, double servicePrice)
    : id (id)
    , receiptOrderID (receiptOrderID)
    , status (status)
    , comment (comment)
    , startDate (startDate)
    , endDate (endDate)
    , technicianID (technicianID)
    , usedParts ()
    , servicePrice (servicePrice) {}

int WorkOrder::getId () const {
    return id;
}
int WorkOrder::getReceiptOrderID () const {
    return receiptOrderID;
}
WorkOrderStatus WorkOrder::getStatus () const {
    return status;
}
std::string WorkOrder::getComment () const {
    return comment;
}
std::time_t WorkOrder::getStartDate () const {
    return startDate;
}
std::time_t WorkOrder::getEndDate () const {
    return endDate;
}
int WorkOrder::getTechnicianID () const {
    return technicianID;
}
std::map<int, int> WorkOrder::getUsedParts () const {
    return usedParts;
}
double WorkOrder::getServicePrice () const {
    return servicePrice;
}

void WorkOrder::setId (int v) {
    if (!isValidId (v)) throw std::invalid_argument ("ID radnog naloga mora biti izmedju 0 i 2147483646");
    id = v;
}

void WorkOrder::setReceiptOrderID (int v) {
    if (!isValidReceiptOrderID (v)) throw std::invalid_argument ("ID prijemnog naloga mora biti izmedju 0 i 2147483646");
    receiptOrderID = v;
}
void WorkOrder::setStatus (WorkOrderStatus v) {
    if (!isValidStatus (v)) throw std::invalid_argument ("Neispravan status popravke");
    status = v;
}
void WorkOrder::setComment (const std::string& v) {
    if (!isValidComment (v)) throw std::invalid_argument ("Komentar mora biti kraci od 100 karaktera");
    comment = v;
}
void WorkOrder::setStartDate (std::time_t v) {
    if (!isValidStartDate (v)) throw std::invalid_argument ("Neispravan datum pocetka popravke");
    startDate = v;
}
void WorkOrder::setEndDate (std::time_t v) {
    if (!isValidEndDate (v)) throw std::invalid_argument ("Neispravan datum zavrsetka popravke");
    endDate = v;
}
void WorkOrder::setTechnicianID (int v) {
    if (!isValidTechnicianID (v)) throw std::invalid_argument ("ID tehnicara mora biti izmedju 0 i 2147483646");
    technicianID = v;
}
void WorkOrder::setUsedParts (const std::map<int, int>& v) {
    if (!isValidUsedParts (v)) throw std::invalid_argument ("ID dijela i kolicina moraju biti pozitivni");
    usedParts = v;
}
void WorkOrder::setServicePrice (double v) {
    if (!isValidServicePrice (v)) throw std::invalid_argument ("Cijena servisiranja mora biti nenegativna");
    servicePrice = v;
}

bool isValidId (int id) {
    return id > 0 && id < INT_MAX;
}
bool isValidReceiptOrderID (int receiptOrderID) {
    return receiptOrderID > 0 && receiptOrderID < INT_MAX;
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
bool isValidEndDate (std::time_t endDate) {
    std::time_t now = std::time (nullptr);
    return endDate >= 0 && endDate <= now;
}
bool isValidTechnicianID (int technicianID) {
    return technicianID > 0 && technicianID < INT_MAX;
}
bool isValidUsedParts (const std::map<int, int>& usedParts) {
    for (const auto& part : usedParts) {
        if (part.first <= 0 || part.second <= 0) return false;
    }
    return true;
}
bool isValidServicePrice (double servicePrice) {
    return servicePrice >= 0.0;
}