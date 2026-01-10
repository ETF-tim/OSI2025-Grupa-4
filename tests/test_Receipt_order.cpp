#include <gtest/gtest.h>

#include <algorithm>
#include <climits>
#include <string>

#include "../include/common/models/Receipt_order.hpp"
#include "../include/common/util/Validate.hpp"

TEST (ReceiptOrder_DefaultConstructor, InitializesDefaults) {
    ReceiptOrder r;
    EXPECT_EQ (r.getId (), -1);
    EXPECT_EQ (r.getUserId (), -1);
    EXPECT_EQ (r.getDeviceIMEI (), "");
    EXPECT_EQ (r.getDescription (), "");
    EXPECT_DOUBLE_EQ (r.getPriceAssessment (), -1.0);
}

TEST (ReceiptOrder_ParamConstructor, InitializesValues) {
    std::string imei = "123456789012345";
    std::string desc = "Test description";
    ReceiptOrder r (5, 10, imei, desc, 99.99);
    EXPECT_EQ (r.getId (), 5);
    EXPECT_EQ (r.getUserId (), 10);
    EXPECT_EQ (r.getDeviceIMEI (), imei);
    EXPECT_EQ (r.getDescription (), desc);
    EXPECT_DOUBLE_EQ (r.getPriceAssessment (), 99.99);
}

TEST (ReceiptOrder_Setters_ValidValues, AcceptsBoundaryValues) {
    ReceiptOrder r;

    r.setId (0);
    EXPECT_EQ (r.getId (), 0);
    r.setId (INT_MAX - 1);
    EXPECT_EQ (r.getId (), INT_MAX - 1);

    r.setUserId (0);
    EXPECT_EQ (r.getUserId (), 0);
    r.setUserId (INT_MAX - 1);
    EXPECT_EQ (r.getUserId (), INT_MAX - 1);

    std::string imei (15, '9');
    r.setDeviceIMEI (imei);
    EXPECT_EQ (r.getDeviceIMEI (), imei);

    std::string desc200 (200, 'a');
    r.setDescription (desc200);
    EXPECT_EQ (r.getDescription (), desc200);

    r.setPriceAssessment (0.0);
    EXPECT_DOUBLE_EQ (r.getPriceAssessment (), 0.0);
    r.setPriceAssessment (1234.56);
    EXPECT_DOUBLE_EQ (r.getPriceAssessment (), 1234.56);
}

TEST (ReceiptOrder_Setters_InvalidId, ThrowsOnOutOfRange) {
    ReceiptOrder r;
    EXPECT_THROW (r.setId (-1), std::invalid_argument);
    EXPECT_THROW (r.setId (INT_MAX), std::invalid_argument);

    EXPECT_THROW (r.setUserId (-5), std::invalid_argument);
    EXPECT_THROW (r.setUserId (INT_MAX), std::invalid_argument);
}

TEST (ReceiptOrder_Setters_InvalidDeviceIMEI, ThrowsOnWrongLengths) {
    ReceiptOrder r;
    EXPECT_THROW (r.setDeviceIMEI ("123"), std::invalid_argument);
    EXPECT_THROW (r.setDeviceIMEI (std::string (16, '1')), std::invalid_argument);
    EXPECT_THROW (r.setDeviceIMEI (""), std::invalid_argument);
}

TEST (ReceiptOrder_Setters_InvalidDescription, ThrowsOnEmptyOrTooLong) {
    ReceiptOrder r;
    EXPECT_THROW (r.setDescription (""), std::invalid_argument);
    EXPECT_THROW (r.setDescription (std::string (201, 'x')), std::invalid_argument);

    EXPECT_NO_THROW (r.setDescription (std::string (200, 'y')));
}

TEST (ReceiptOrder_Setters_InvalidPriceAssessment, ThrowsOnNegative) {
    ReceiptOrder r;
    EXPECT_THROW (r.setPriceAssessment (-0.0001), std::invalid_argument);
    EXPECT_THROW (r.setPriceAssessment (-100.0), std::invalid_argument);
    EXPECT_NO_THROW (r.setPriceAssessment (0.0));
    EXPECT_NO_THROW (r.setPriceAssessment (1.23));
}

TEST (ReceiptOrder_CopyConstructor, CopiesAllFields) {
    std::string imei = "123456789012345";
    std::string desc = "Copy constructor test";
    ReceiptOrder orig (7, 20, imei, desc, 50.5);

    ReceiptOrder cpy (orig);
    EXPECT_EQ (cpy.getId (), orig.getId ());
    EXPECT_EQ (cpy.getUserId (), orig.getUserId ());
    EXPECT_EQ (cpy.getDeviceIMEI (), orig.getDeviceIMEI ());
    EXPECT_EQ (cpy.getDescription (), orig.getDescription ());
    EXPECT_DOUBLE_EQ (cpy.getPriceAssessment (), orig.getPriceAssessment ());
}

TEST (ReceiptOrder_AssignmentOperator, CopiesAllFields) {
    ReceiptOrder a;
    ReceiptOrder b (9, 30, "987654321098765", "Assignment test", 75.25);

    a = b;
    EXPECT_EQ (a.getId (), b.getId ());
    EXPECT_EQ (a.getUserId (), b.getUserId ());
    EXPECT_EQ (a.getDeviceIMEI (), b.getDeviceIMEI ());
    EXPECT_EQ (a.getDescription (), b.getDescription ());
    EXPECT_DOUBLE_EQ (a.getPriceAssessment (), b.getPriceAssessment ());
}

TEST (ReceiptOrder_MoveConstructor, TransfersValues) {
    ReceiptOrder tmp (11, 40, "111111111111111", "Move ctor test", 100.0);
    ReceiptOrder moved (std::move (tmp));

    EXPECT_EQ (moved.getId (), 11);
    EXPECT_EQ (moved.getUserId (), 40);
    EXPECT_EQ (moved.getDeviceIMEI (), "111111111111111");
    EXPECT_EQ (moved.getDescription (), "Move ctor test");
    EXPECT_DOUBLE_EQ (moved.getPriceAssessment (), 100.0);
}

TEST (ReceiptOrder_MoveAssignment, TransfersValues) {
    ReceiptOrder a;
    ReceiptOrder b (12, 41, "222222222222222", "Move assign test", 150.0);

    a = std::move (b);
    EXPECT_EQ (a.getId (), 12);
    EXPECT_EQ (a.getUserId (), 41);
    EXPECT_EQ (a.getDeviceIMEI (), "222222222222222");
    EXPECT_EQ (a.getDescription (), "Move assign test");
    EXPECT_DOUBLE_EQ (a.getPriceAssessment (), 150.0);
}
