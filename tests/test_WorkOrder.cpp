#include <gtest/gtest.h>
#include <climits>
#include <ctime>
#include <map>
#include "../include/common/models/WorkOrder.hpp"

TEST(WorkOrderGTest, BasicValidSettersGetters)
{
    WorkOrder wo;

    EXPECT_NO_THROW(wo.setId(1));
    EXPECT_EQ(wo.getId(), 1);

    EXPECT_NO_THROW(wo.setReceiptOrderID(2));
    EXPECT_EQ(wo.getReceiptOrderID(), 2);

    EXPECT_NO_THROW(wo.setStatus(WorkOrderStatus::IN_REPAIR));
    EXPECT_EQ(wo.getStatus(), WorkOrderStatus::IN_REPAIR);

    EXPECT_NO_THROW(wo.setComment("Test comment"));
    EXPECT_EQ(wo.getComment(), "Test comment");

    std::time_t now = std::time(nullptr);
    EXPECT_NO_THROW(wo.setStartDate(now - 3600));
    EXPECT_EQ(wo.getStartDate(), now - 3600);

    EXPECT_NO_THROW(wo.setEndDate(now - 1800));
    EXPECT_EQ(wo.getEndDate(), now - 1800);

    EXPECT_NO_THROW(wo.setTechnicianID(10));
    EXPECT_EQ(wo.getTechnicianID(), 10);

    EXPECT_NO_THROW(wo.setServicePrice(250.50));
    EXPECT_DOUBLE_EQ(wo.getServicePrice(), 250.50);
}

TEST(WorkOrderGTest, AllStatusEnums)
{
    WorkOrder wo;

    EXPECT_NO_THROW(wo.setStatus(WorkOrderStatus::IN_DIAGNOSTICS));
    EXPECT_EQ(wo.getStatus(), WorkOrderStatus::IN_DIAGNOSTICS);

    EXPECT_NO_THROW(wo.setStatus(WorkOrderStatus::IN_REPAIR));
    EXPECT_EQ(wo.getStatus(), WorkOrderStatus::IN_REPAIR);

    EXPECT_NO_THROW(wo.setStatus(WorkOrderStatus::WAITING_FOR_PARTS));
    EXPECT_EQ(wo.getStatus(), WorkOrderStatus::WAITING_FOR_PARTS);

    EXPECT_NO_THROW(wo.setStatus(WorkOrderStatus::COMPLETED));
    EXPECT_EQ(wo.getStatus(), WorkOrderStatus::COMPLETED);
}

TEST(WorkOrderGTest, EdgeValidValues)
{
    WorkOrder wo;

    EXPECT_NO_THROW(wo.setId(INT_MAX - 1));
    EXPECT_EQ(wo.getId(), INT_MAX - 1);

    EXPECT_NO_THROW(wo.setReceiptOrderID(INT_MAX - 1));
    EXPECT_EQ(wo.getReceiptOrderID(), INT_MAX - 1);

    EXPECT_NO_THROW(wo.setTechnicianID(INT_MAX - 1));
    EXPECT_EQ(wo.getTechnicianID(), INT_MAX - 1);

    EXPECT_NO_THROW(wo.setServicePrice(0.0));
    EXPECT_DOUBLE_EQ(wo.getServicePrice(), 0.0);

    std::string maxComment(100, 'A');
    EXPECT_NO_THROW(wo.setComment(maxComment));
    EXPECT_EQ(wo.getComment(), maxComment);

    EXPECT_NO_THROW(wo.setComment(""));
    EXPECT_EQ(wo.getComment(), "");
}

TEST(WorkOrderGTest, InvalidIdThrows)
{
    WorkOrder wo;
    EXPECT_THROW(wo.setId(0), std::invalid_argument);
    EXPECT_THROW(wo.setId(-1), std::invalid_argument);
    EXPECT_THROW(wo.setId(INT_MAX), std::invalid_argument);
}

TEST(WorkOrderGTest, InvalidReceiptOrderIDThrows)
{
    WorkOrder wo;
    EXPECT_THROW(wo.setReceiptOrderID(0), std::invalid_argument);
    EXPECT_THROW(wo.setReceiptOrderID(-5), std::invalid_argument);
    EXPECT_THROW(wo.setReceiptOrderID(INT_MAX), std::invalid_argument);
}

TEST(WorkOrderGTest, InvalidStatusThrows)
{
    WorkOrder wo;
    EXPECT_THROW(wo.setStatus(static_cast<WorkOrderStatus>(-1)), std::invalid_argument);
    EXPECT_THROW(wo.setStatus(static_cast<WorkOrderStatus>(999)), std::invalid_argument);
}

TEST(WorkOrderGTest, InvalidCommentThrows)
{
    WorkOrder wo;
    std::string tooLong(101, 'A');
    EXPECT_THROW(wo.setComment(tooLong), std::invalid_argument);
}

TEST(WorkOrderGTest, InvalidTechnicianIDThrows)
{
    WorkOrder wo;
    EXPECT_THROW(wo.setTechnicianID(0), std::invalid_argument);
    EXPECT_THROW(wo.setTechnicianID(-1), std::invalid_argument);
    EXPECT_THROW(wo.setTechnicianID(INT_MAX), std::invalid_argument);
}

TEST(WorkOrderGTest, InvalidServicePriceThrows)
{
    WorkOrder wo;
    EXPECT_THROW(wo.setServicePrice(-0.0001), std::invalid_argument);
    EXPECT_THROW(wo.setServicePrice(-100.0), std::invalid_argument);
}

TEST(WorkOrderGTest, ValidStartDatePastAndPresent)
{
    WorkOrder wo;
    std::time_t now = std::time(nullptr);

    EXPECT_NO_THROW(wo.setStartDate(now));
    EXPECT_EQ(wo.getStartDate(), now);

    EXPECT_NO_THROW(wo.setStartDate(now - 86400));
    EXPECT_EQ(wo.getStartDate(), now - 86400);

    EXPECT_NO_THROW(wo.setStartDate(0));
    EXPECT_EQ(wo.getStartDate(), 0);
}

TEST(WorkOrderGTest, InvalidStartDateFutureThrows)
{
    WorkOrder wo;
    std::time_t future = std::time(nullptr) + 3600;
    EXPECT_THROW(wo.setStartDate(future), std::invalid_argument);
}

TEST(WorkOrderGTest, ValidEndDate)
{
    WorkOrder wo;
    std::time_t now = std::time(nullptr);

    EXPECT_NO_THROW(wo.setEndDate(now));
    EXPECT_EQ(wo.getEndDate(), now);

    EXPECT_NO_THROW(wo.setEndDate(now - 3600));
    EXPECT_EQ(wo.getEndDate(), now - 3600);

    EXPECT_NO_THROW(wo.setEndDate(0));
    EXPECT_EQ(wo.getEndDate(), 0);
}

TEST(WorkOrderGTest, InvalidEndDateFutureThrows)
{
    WorkOrder wo;
    std::time_t future = std::time(nullptr) + 3600;
    EXPECT_THROW(wo.setEndDate(future), std::invalid_argument);
}

TEST(WorkOrderGTest, InvalidEndDateNegativeThrows)
{
    WorkOrder wo;
    EXPECT_THROW(wo.setEndDate(-1), std::invalid_argument);
}