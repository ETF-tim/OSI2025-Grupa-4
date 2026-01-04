#include <iostream>
#include "../include/common/models/Bill.hpp"

int main()
{
    Bill bill(1, 1, PaymentMethod::CASH, 3.2);
    std::cout << bill.getId() << std::endl;
}