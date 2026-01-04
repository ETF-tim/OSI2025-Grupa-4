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
Part(int id,const std::string& name,int amount,int criticalAmount,double prize)
   :id(id),name(name),amount(amount),criticalAmount(criticalAmount),prize(prize){}

   int getId() const noexcept {return id;}
   void setId(int v) noexcept {id=v;}

   const std::string& getName() const noexcept {return name;}
   void setName(const std::string& v) noexcept {name=v;}
  
   int getAmount() const noexcept {return amount;}
   void setAmount(int v) noexcept {amount=v;}

   int getCriticalAmount() const noexcept {return criticalAmount;}
   void setCriticalAmount(int v) noexcept {criticalAmount=v;}

   double getPrize() const noexcept {return prize;}
   void setPrize(double v) noexcept {prize=v;}

};


bool isValidId(int id);
bool isValidName(const std::string& name);
bool isValidAmount(int amount);
bool isValidCriticalAmount(int criticalAmount);
bool isValidPrice(double price);