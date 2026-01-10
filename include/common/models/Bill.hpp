#pragma once

enum class PaymentMethod { UNKNOWN, CASH, CREDIT_CARD, DIGITAL_WALLET };

class Bill {
    private:
        int id;
        int workOrderID;
        PaymentMethod paymentMethod;
        double price;

    public:
        Bill ();
        Bill (int id, int workOrderID, PaymentMethod paymentMethod, double price);

        int getId () const;
        int getWorkOrderID () const;
        PaymentMethod getPaymentMethod () const;
        double getPrice () const;

        void setId (int id);
        void setWorkOrderID (int workOrderID);
        void setPaymentMethod (PaymentMethod paymentMethod);
        void setPrice (double price);
};
