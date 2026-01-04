#pragma once

class ServiceReport
{
private:
    int id;
    int workOrderID;

public:
    ServiceReport(int id, int workOrderID)
        : id(id), workOrderID(workOrderID) {}

    int getId() const;
    int getWorkOrderID() const;

    void setId(int id);
    void setWorkOrderID(int workOrderID);
};

// Validation functions for all attributes of class ServiceReport
bool isValidId(int id);
bool isValidWorkOrderID(int workOrderID);