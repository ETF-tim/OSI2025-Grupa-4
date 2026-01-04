#pragma once

enum class PaymentMethod
{
    CASH,
    CREDIT_CARD,
    DIGITAL_WALLET,
    UNKNOWN
};

class Bill
{
private:
    int id;
    int receiptOrderID;
    PaymentMethod paymentMethod;
    double price;

public:
    Bill(int id, int receiptOrderID, PaymentMethod paymentMethod, double price)
        : id(id), receiptOrderID(receiptOrderID), paymentMethod(paymentMethod), price(price) {}
    Bill() : id(-1), receiptOrderID(-1), paymentMethod(PaymentMethod::UNKNOWN), price(-1.0) {};

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
bool isValidReceiptOrderID(int receiptOrderID);
bool isValidPaymentMethod(PaymentMethod paymentMethod);
bool isValidPrice(double price);