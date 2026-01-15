#pragma once
#include <string>

enum class PaymentMethod;

class BillManager {
    public:
        int createBill (int workOrderID, PaymentMethod paymentMethod, double price);
        void listBills ();
        void deleteBill ();
        bool searchForBill (int billId);
        void generateBillTXTFile (int billId, int workOrderId);
        void mainBillsManagement ();

    private:
        std::string paymentMethodToString (int paymentMethodInt);
};
