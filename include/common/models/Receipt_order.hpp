#pragma once
#include <string>

class ReceiptOrder
{
private:
    int id;
    int userID;
    std::string deviceIMEI;
    std::string description;
    double priceAssessment;

public:
    ReceiptOrder();
    ReceiptOrder(int id, int userID, const std::string &deviceIMEI,
                 const std::string &description, double priceAssessment);

    int getId() const;
    int getUserId() const;
    const std::string &getDeviceIMEI() const;
    const std::string &getDescription() const;
    double getPriceAssessment() const;

    void setId(int v);
    void setUserId(int v);
    void setDeviceIMEI(const std::string &v);
    void setDescription(const std::string &v);
    void setPriceAssessment(double v);
};

bool isValidId(int id);
bool isValidDeviceIMEI(const std::string &deviceIMEI);
bool isValidDescription(const std::string &description);
bool isValidPriceAssessment(double priceAssessment);