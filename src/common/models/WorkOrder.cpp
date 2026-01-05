#include "../../../include/common/models/WorkOrder.hpp"

#include <limits>
#include <stdexcept>

#include "../../../include/common/util/Validate.hpp"

WorkOrder::WorkOrder ()
    : id (-1)
    , receiptOrderID (-1)
    , status (WorkOrderStatus::UNKNOWN)
    , comment ("")
    , startDate (0)
    , endDate (0)
    , technicianID (-1)
    , servicePrice (-1) {}

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

// Getters
int WorkOrder::getId () const {
    return id;
}
int WorkOrder::getReceiptOrderID () const {
    return receiptOrderID;
}
WorkOrderStatus WorkOrder::getStatus () const {
    return status;
}
const std::string& WorkOrder::getComment () const {
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
const std::map<int, int>& WorkOrder::getUsedParts () const {
    return usedParts;
}
double WorkOrder::getServicePrice () const {
    return servicePrice;
}

// Setters with validation
void WorkOrder::setId (int v) {
    if (!Validate::isValidId (v)) throw std::invalid_argument ("ID radnog naloga mora biti izmedju 0 i 2147483646");
    id = v;
}
void WorkOrder::setReceiptOrderID (int v) {
    if (!Validate::isValidId (v)) throw std::invalid_argument ("ID prijemnog naloga mora biti izmedju 0 i 2147483646");
    receiptOrderID = v;
}
void WorkOrder::setStatus (WorkOrderStatus v) {
    if (!Validate::isValidStatus (v)) throw std::invalid_argument ("Neispravan status popravke");
    status = v;
}
void WorkOrder::setComment (const std::string& v) {
    if (!Validate::isValidComment (v)) throw std::invalid_argument ("Komentar mora biti kraci od 100 karaktera");
    comment = v;
}
void WorkOrder::setStartDate (std::time_t v) {
    if (!Validate::isValidStartDate (v)) throw std::invalid_argument ("Neispravan datum pocetka popravke");
    startDate = v;
}
void WorkOrder::setEndDate (std::time_t v) {
    if (!Validate::isValidEndDate (startDate, v)) throw std::invalid_argument ("Neispravan datum zavrsetka popravke");
    endDate = v;
}
void WorkOrder::setTechnicianID (int v) {
    if (!Validate::isValidId (v)) throw std::invalid_argument ("ID tehnicara mora biti izmedju 0 i 2147483646");
    technicianID = v;
}
void WorkOrder::setUsedParts (const std::map<int, int>& v) {
    if (!Validate::isValidUsedParts (v)) throw std::invalid_argument ("ID dijela i kolicina moraju biti pozitivni");
    usedParts = v;
}
void WorkOrder::setServicePrice (double v) {
    if (!Validate::isValidPrice (v)) throw std::invalid_argument ("Cijena servisiranja mora biti nenegativna");
    servicePrice = v;
}
