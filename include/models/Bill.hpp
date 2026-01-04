#pragma once

enum class PaymentMethod
{
    UNKNOWN,
    CASH,
    CREDIT_CARD,
    DIGITAL_WALLET
};

class Bill
{
private:
    int id;
    int receiptOrderID;
    PaymentMethod paymentMethod;
    double price;

public:
    Bill();
    Bill(int id, int receiptOrderID, PaymentMethod paymentMethod, double price);

    int getId() const;
    int getReceiptOrderID() const;
    PaymentMethod getPaymentMethod() const;
    double getPrice() const;

    void setId(int id);
    void setReceiptOrderID(int receiptOrderID);
    void setPaymentMethod(PaymentMethod paymentMethod);
    void setPrice(double price);
};

bool isValidId(int id);
bool isValidPaymentMethod(PaymentMethod paymentMethod);
bool isValidPrice(double price);
