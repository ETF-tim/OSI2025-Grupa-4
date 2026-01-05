#pragma once

class ServiceReport
{
private:
    int id;
    int workOrderID;

public:
    ServiceReport(int id, int workOrderID)
        : id(id), workOrderID(workOrderID) {}
    ServiceReport() : id(-1), workOrderID(-1) {}
    int getId() const;
    int getWorkOrderID() const;

    void setId(int id);
    void setWorkOrderID(int workOrderID);
};

bool isValidId(int id);
bool isValidWorkOrderID(int workOrderID);