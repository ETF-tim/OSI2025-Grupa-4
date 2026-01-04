#include "../../include/models/Receipt_order.hpp"
#include <stdexcept>
#include <limits>

Receipt_order::Receipt_order(int id,int clientId,int workerId,const std::string& date,double totalPrice)
   :id(id),clientId(clientId),workerId(workerId),date(date),totalPrice(totalPrice){}


int Receipt_order::getId() const noexcept