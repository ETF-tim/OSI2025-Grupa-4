#include <gtest/gtest.h>
#include "../include/common/models/Receipt_order.hpp"
#include <climits>
#include <string>
#include <algorithm>

TEST(ReceiptOrder_DefaultConstructor, InitializesDefaults)
{
    ReceiptOrder r;
    EXPECT_EQ(r.getId(), -1);
    EXPECT_EQ(r.getUserId(), -1);
    EXPECT_EQ(r.getDeviceIMEI(), "");
    EXPECT_EQ(r.getDescription(), "");
    EXPECT_DOUBLE_EQ(r.getPriceAssessment(), -1.0);
}

TEST(ReceiptOrder_ParamConstructor, InitializesValues)
{
    std::string imei = "123456789012345";
    std::string desc = "Test description";
    ReceiptOrder r(5, 10, imei, desc, 99.99);
    EXPECT_EQ(r.getId(), 5);
    EXPECT_EQ(r.getUserId(), 10);
    EXPECT_EQ(r.getDeviceIMEI(), imei);
    EXPECT_EQ(r.getDescription(), desc);
    EXPECT_DOUBLE_EQ(r.getPriceAssessment(), 99.99);
}

TEST(ReceiptOrder_Setters_ValidValues, AcceptsBoundaryValues)
{
    ReceiptOrder r;

    r.setId(0);
    EXPECT_EQ(r.getId(), 0);
    r.setId(INT_MAX - 1);
    EXPECT_EQ(r.getId(), INT_MAX - 1);

    r.setUserId(0);
    EXPECT_EQ(r.getUserId(), 0);
    r.setUserId(INT_MAX - 1);
    EXPECT_EQ(r.getUserId(), INT_MAX - 1);

    std::string imei(15, '9');
    r.setDeviceIMEI(imei);
    EXPECT_EQ(r.getDeviceIMEI(), imei);

    std::string desc200(200, 'a');
    r.setDescription(desc200);
    EXPECT_EQ(r.getDescription(), desc200);

    r.setPriceAssessment(0.0);
    EXPECT_DOUBLE_EQ(r.getPriceAssessment(), 0.0);
    r.setPriceAssessment(1234.56);
    EXPECT_DOUBLE_EQ(r.getPriceAssessment(), 1234.56);
}

TEST(ReceiptOrder_Setters_InvalidId, ThrowsOnOutOfRange)
{
    ReceiptOrder r;
    EXPECT_THROW(r.setId(-1), std::invalid_argument);
    EXPECT_THROW(r.setId(INT_MAX), std::invalid_argument);

    EXPECT_THROW(r.setUserId(-5), std::invalid_argument);
    EXPECT_THROW(r.setUserId(INT_MAX), std::invalid_argument);
}

TEST(ReceiptOrder_Setters_InvalidDeviceIMEI, ThrowsOnWrongLengths)
{
    ReceiptOrder r;
    EXPECT_THROW(r.setDeviceIMEI("123"), std::invalid_argument);
    EXPECT_THROW(r.setDeviceIMEI(std::string(16, '1')), std::invalid_argument);
    EXPECT_THROW(r.setDeviceIMEI(""), std::invalid_argument);
}

TEST(ReceiptOrder_Setters_InvalidDescription, ThrowsOnEmptyOrTooLong)
{
    ReceiptOrder r;
    EXPECT_THROW(r.setDescription(""), std::invalid_argument);
    EXPECT_THROW(r.setDescription(std::string(201, 'x')), std::invalid_argument);

    EXPECT_NO_THROW(r.setDescription(std::string(200, 'y')));
}

TEST(ReceiptOrder_Setters_InvalidPriceAssessment, ThrowsOnNegative)
{
    ReceiptOrder r;
    EXPECT_THROW(r.setPriceAssessment(-0.0001), std::invalid_argument);
    EXPECT_THROW(r.setPriceAssessment(-100.0), std::invalid_argument);
    EXPECT_NO_THROW(r.setPriceAssessment(0.0));
    EXPECT_NO_THROW(r.setPriceAssessment(1.23));
}