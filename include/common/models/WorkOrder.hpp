#pragma once

#include <string>
#include <map>
#include <ctime>

enum class WorkOrderStatus
{
    IN_DIAGNOSTICS,
    IN_REPAIR,
    WAITING_FOR_PARTS,
    COMPLETED,
    UNKNOWN
};

class WorkOrder
{
private:
    int id;
    int receiptOrderID;
    WorkOrderStatus status;
    std::string comment;
    std::time_t startDate;
    std::time_t endDate;
    int technicianID;
    std::map<int, int> usedParts;
    double servicePrice;

public:
    WorkOrder() : id(-1), receiptOrderID(-1), status(WorkOrderStatus::UNKNOWN), comment("N/A"), startDate() {};
    WorkOrder(int id, int receiptOrderID, WorkOrderStatus status, const std::string &comment,
              std::time_t startDate, std::time_t endDate, int technicianID, double servicePrice);

    int getId() const;
    int getReceiptOrderID() const;
    WorkOrderStatus getStatus() const;
    std::string getComment() const;
    std::time_t getStartDate() const;
    std::time_t getEndDate() const;
    int getTechnicianID() const;
    std::map<int, int> getUsedParts() const;
    double getServicePrice() const;

    void setId(int id);
    void setReceiptOrderID(int receiptOrderID);
    void setStatus(WorkOrderStatus status);
    void setComment(const std::string &comment);
    void setStartDate(std::time_t startDate);
    void setEndDate(std::time_t endDate);
    void setTechnicianID(int technicianID);
    void setUsedParts(const std::map<int, int> &usedParts);
    void setServicePrice(double servicePrice);
};

bool isValidId(int id);
bool isValidReceiptOrderID(int receiptOrderID);
bool isValidStatus(WorkOrderStatus status);
bool isValidComment(const std::string &comment);
bool isValidStartDate(std::time_t startDate);
bool isValidEndDate(std::time_t endDate);
bool isValidTechnicianID(int technicianID);
bool isValidUsedParts(const std::map<int, int> &usedParts);
bool isValidServicePrice(double servicePrice);
