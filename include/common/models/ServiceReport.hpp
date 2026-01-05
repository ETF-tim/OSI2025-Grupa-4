#pragma once

class ServiceReport
{
private:
    int id;
    int workOrderID;

public:
    ServiceReport();
    ServiceReport(int id, int workOrderID);

    int getId() const;
    int getWorkOrderID() const;

    void setId(int id);
    void setWorkOrderID(int workOrderID);
};

bool isValidId(int id);
