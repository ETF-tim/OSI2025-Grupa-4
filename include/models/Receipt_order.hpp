#pragma once
#include <string>

class ReceiptOrder
{
    private:
    int id;
    int userID;
    std::string deviceIMEI;
    std::string description;
    double priceAssesment;

    public:
    ReceiptOrder()=default;
    ReceiptOrder(int id, int userID,const std::string& deviceIMEI,
        const std::string& description, double priceAssesment);

        int getId()const noexcept{return id;}
        void setId(int v) noexcept{id=v;}
  
        int getUserId()const noexcept{return userID;}
        void setUserId(int v) noexcept{userID=v;}

        const std::string getDeviceIMEI() const noexcept{return deviceIMEI;}
        void setDeviceIMEI(const std::string& v) noexcept{deviceIMEI=v;}

        const std::string getDescription() const noexcept{return description;}
        void setDescription(const std::string& v) noexcept{description=v;}

        double getPriceAssesment() const noexcept{return priceAssesment;}
        void setPriceAssesment(double v) noexcept{priceAssesment=v;}

};

bool isValidId(int id);
bool isValidUserID(int userID);
bool isValidDeviceIMEI(const std::string& deviceIMEI);
bool isValidDescription(const std::string& description);
bool isValidPriceAssesment(double priceAssesment);