#pragma once
#include <string>

class Part
{
private:
int id;
std::string name;
int amount;
int criticalAmount;
double prize;

public:
Part()=default;
Part(int id,const std::string& name,int amount,int criticalAmount,double prize){};

   int getId() const noexcept {return id;}
   void setId(int v) ;

   const std::string& getName() const noexcept {return name;}
   void setName(const std::string& v);
  
   int getAmount() const noexcept {return amount;}
   void setAmount(int v) noexcept;

   int getCriticalAmount() const noexcept {return criticalAmount;}
   void setCriticalAmount(int v) noexcept;

   double getPrize() const noexcept {return prize;}
   void setPrize(double v) noexcept;

};


bool isValidId(int id);
bool isValidName(const std::string& name);
bool isValidAmount(int amount);
bool isValidCriticalAmount(int criticalAmount);
bool isValidPrice(double price);