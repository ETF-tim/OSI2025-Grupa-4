#include "../../include/models/Part.hpp"
#include <stdexcept>
#include <limits>

Part::Part(int id,const std::string& name,int amount,int criticalAmount,double prize)
   :id(id),name(name),amount(amount),criticalAmount(criticalAmount),prize(prize){}

   
   int Part::getId() const noexcept 
   {
    return id;
   }
   
   void Part::setId(int v) noexcept
    {
       if(!isValidId(v))
        throw std::invalid_argument("Neodgovarajuci id");
        id = v;
    
    }

   const std::string& Part::getName() const noexcept 
   {
    return name;
   }

 void Part::setName(const std::string& v)
{
   if(!isValidName(v))
        throw std::invalid_argument("Neodgovarajuci naziv");

        name = v;
    
}
  
   int Part::getAmount() const noexcept
    {
     return amount;
    }

   void Part::setAmount(int v) noexcept
    {
          if(!isValidAmount(v))
        throw std::invalid_argument("Neodgovarajuca kolicina");

        amount = v;
    }

   int Part::getCriticalAmount() const noexcept
    {
        return criticalAmount;
    }

   void Part::setCriticalAmount(int v) noexcept 
   {
     if(!isValidCriticalAmount(v))
        throw std::invalid_argument("Neodgovarajuca kriticna kolicina");

        criticalAmount = v;
   }

   double Part::getPrize() const noexcept
    {
     return prize;
    }

   void Part::setPrize(double v) noexcept
    {
           if(!isValidPrice(v))
        throw std::invalid_argument("Neodgovarajuca cijena");

        prize = v;
    }

    

    bool isValidId(int id)
{
    return id > 0;
}

bool isValidName(const std::string& name)
{
    return !name.empty() && name.length() <= 100;
}

bool isValidAmount(int amount)
{
    return amount >= 0;  // Količina ne može biti negativna
}

bool isValidCriticalAmount(int criticalAmount)
{
    return criticalAmount >= 0;  // Kritična količina ne može biti negativna
}

bool isValidPrice(double price)
{
    return price > 0.0;  // Cijena mora biti pozitivna
}