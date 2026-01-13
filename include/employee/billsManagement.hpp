#pragma once
#include <string>

enum class PaymentMethod;

class BillManager {
    public:
        void createBill (int workOrderID, PaymentMethod paymentMethod, double price);
        void listBills ();
        void deleteBill ();
        bool searchForBill (int billId);
        void mainBillsManagement ();
    private:
        std::string paymentMethodToString (int paymentMethodInt);
};
