#include "../../../include/common/models/Receipt_order.hpp"
#include <stdexcept>
#include <limits>

ReceiptOrder::ReceiptOrder()
    : id(-1), userID(-1), deviceIMEI(""), description(""), priceAssessment(-1) {}

ReceiptOrder::ReceiptOrder(int id, int userID, const std::string &deviceIMEI,
                           const std::string &description, double priceAssessment)
    : id(id), userID(userID), deviceIMEI(deviceIMEI),
      description(description), priceAssessment(priceAssessment) {}

int ReceiptOrder::getId() const { return id; }
int ReceiptOrder::getUserId() const { return userID; }
const std::string &ReceiptOrder::getDeviceIMEI() const { return deviceIMEI; }
const std::string &ReceiptOrder::getDescription() const { return description; }
double ReceiptOrder::getPriceAssessment() const { return priceAssessment; }

void ReceiptOrder::setId(int v)
{
   if (!isValidId(v))
   {
      throw std::invalid_argument("ID prijemnog naloga mora biti izmedju 0 i 2147483646");
   }
   id = v;
}

void ReceiptOrder::setUserId(int v)
{
   if (!isValidId(v))
   {
      throw std::invalid_argument("ID korisnickog naloga mora biti izmedju 0 i 2147483646");
   }
   userID = v;
}

void ReceiptOrder::setDeviceIMEI(const std::string &v)
{
   if (!isValidDeviceIMEI(v))
   {
      throw std::invalid_argument("IMEI uredjaja mora biti duzine 15 karaktera");
   }
   deviceIMEI = v;
}

void ReceiptOrder::setDescription(const std::string &v)
{
   if (!isValidDescription(v))
   {
      throw std::invalid_argument("Opis mora biti duzine do 200 karaktera");
   }
   description = v;
}

void ReceiptOrder::setPriceAssessment(double v)
{
   if (!isValidPriceAssessment(v))
   {
      throw std::invalid_argument("Procjena cijene mora biti nenegativna");
   }
   priceAssessment = v;
}

bool isValidId(int id)
{
   return id >= 0 && id < INT_MAX;
}
bool isValidDeviceIMEI(const std::string &deviceIMEI)
{
   return deviceIMEI.length() == 15;
}
bool isValidDescription(const std::string &description)
{
   return !description.empty() && description.length() <= 200;
}
bool isValidPriceAssessment(double priceAssessment)
{
   return priceAssessment >= 0.0;
}