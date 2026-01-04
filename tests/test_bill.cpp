#include <gtest/gtest.h>
#include <climits>
#include <cmath>
#include <limits>
#include "../include/common/models/Bill.hpp"

TEST(BillGTest, BasicValidSettersGetters)
{
    Bill b;
    EXPECT_NO_THROW(b.setId(1));
    EXPECT_EQ(b.getId(), 1);

    EXPECT_NO_THROW(b.setReceiptOrderID(2));
    EXPECT_EQ(b.getReceiptOrderID(), 2);

    EXPECT_NO_THROW(b.setPaymentMethod(PaymentMethod::CASH));
    EXPECT_EQ(b.getPaymentMethod(), PaymentMethod::CASH);

    const double priceVal = 123.45;
    EXPECT_NO_THROW(b.setPrice(priceVal));
    EXPECT_NEAR(b.getPrice(), priceVal, 1e-9);
}

TEST(BillGTest, EdgeValidValues)
{
    Bill b;
    EXPECT_NO_THROW(b.setId(INT_MAX - 1));
    EXPECT_EQ(b.getId(), INT_MAX - 1);

    EXPECT_NO_THROW(b.setReceiptOrderID(INT_MAX - 1));
    EXPECT_EQ(b.getReceiptOrderID(), INT_MAX - 1);

    EXPECT_NO_THROW(b.setPrice(0.0));
    EXPECT_DOUBLE_EQ(b.getPrice(), 0.0);

    const double largePrice = 1e300;
    EXPECT_NO_THROW(b.setPrice(largePrice));
    EXPECT_DOUBLE_EQ(b.getPrice(), largePrice);

    EXPECT_NO_THROW(b.setPrice(std::numeric_limits<double>::infinity()));
    EXPECT_TRUE(std::isinf(b.getPrice()) && b.getPrice() > 0);
}

TEST(BillGTest, InvalidIdThrows)
{
    Bill b;
    EXPECT_THROW(b.setId(0), std::invalid_argument);
    EXPECT_THROW(b.setId(-1), std::invalid_argument);
    EXPECT_THROW(b.setId(INT_MAX), std::invalid_argument);
}

TEST(BillGTest, InvalidReceiptOrderIDThrows)
{
    Bill b;
    EXPECT_THROW(b.setReceiptOrderID(0), std::invalid_argument);
    EXPECT_THROW(b.setReceiptOrderID(-5), std::invalid_argument);
    EXPECT_THROW(b.setReceiptOrderID(INT_MAX), std::invalid_argument);
}

TEST(BillGTest, InvalidPaymentMethodThrows)
{
    Bill b;
    EXPECT_THROW(b.setPaymentMethod(static_cast<PaymentMethod>(-1)), std::invalid_argument);
    EXPECT_THROW(b.setPaymentMethod(static_cast<PaymentMethod>(9999)), std::invalid_argument);
}

TEST(BillGTest, InvalidPriceThrows)
{
    Bill b;
    EXPECT_THROW(b.setPrice(-0.0001), std::invalid_argument);
    EXPECT_THROW(b.setPrice(-1.0), std::invalid_argument);

    double nan = std::numeric_limits<double>::quiet_NaN();
    EXPECT_THROW(b.setPrice(nan), std::invalid_argument);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}