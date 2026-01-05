#pragma once
#include <string>

class Part
{
private:
   int id;
   std::string name;
   int amount;
   int criticalAmount;
   double price;

public:
   Part();
   Part(int id, const std::string &name, int amount, int criticalAmount, double price);

   int getId() const;
   const std::string &getName() const;
   int getAmount() const;
   int getCriticalAmount() const;
   double getPrice() const;

   void setId(int v);
   void setName(const std::string &v);
   void setAmount(int v);
   void setCriticalAmount(int v);
   void setPrice(double v);
};

bool isValidId(int id);
bool isValidName(const std::string &name);
bool isValidAmount(int amount);
bool isValidCriticalAmount(int criticalAmount);
bool isValidPrice(double price);