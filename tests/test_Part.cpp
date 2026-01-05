#include <gtest/gtest.h>
#include "../include/common/models/Part.hpp"
#include <climits>
#include <string>

TEST(PartTest, DefaultConstructor)
{
    Part p;
    EXPECT_EQ(p.getId(), -1);
    EXPECT_EQ(p.getName(), "");
    EXPECT_EQ(p.getAmount(), -1);
    EXPECT_EQ(p.getCriticalAmount(), -1);
    EXPECT_DOUBLE_EQ(p.getPrice(), -1.0);
}

TEST(PartTest, ParameterizedConstructorAndGetters)
{
    Part p(1, "Bolt", 10, 2, 3.5);
    EXPECT_EQ(p.getId(), 1);
    EXPECT_EQ(p.getName(), "Bolt");
    EXPECT_EQ(p.getAmount(), 10);
    EXPECT_EQ(p.getCriticalAmount(), 2);
    EXPECT_DOUBLE_EQ(p.getPrice(), 3.5);
}

TEST(PartTest, SettersValidValues)
{
    Part p;
    EXPECT_NO_THROW(p.setId(0));
    EXPECT_EQ(p.getId(), 0);

    EXPECT_NO_THROW(p.setName("Washer"));
    EXPECT_EQ(p.getName(), "Washer");

    EXPECT_NO_THROW(p.setAmount(5));
    EXPECT_EQ(p.getAmount(), 5);

    EXPECT_NO_THROW(p.setCriticalAmount(1));
    EXPECT_EQ(p.getCriticalAmount(), 1);

    EXPECT_NO_THROW(p.setPrice(0.0));
    EXPECT_DOUBLE_EQ(p.getPrice(), 0.0);

    EXPECT_NO_THROW(p.setId(INT_MAX - 1));
    EXPECT_EQ(p.getId(), INT_MAX - 1);
}

TEST(PartTest, SetIdInvalidThrows)
{
    Part p;
    EXPECT_THROW(p.setId(-1), std::invalid_argument);
    EXPECT_THROW(p.setId(INT_MAX), std::invalid_argument);
}

TEST(PartTest, SetNameInvalidThrows)
{
    Part p;
    EXPECT_THROW(p.setName(""), std::invalid_argument);

    std::string longName(101, 'a');
    EXPECT_EQ(longName.size(), 101u);
    EXPECT_THROW(p.setName(longName), std::invalid_argument);

    std::string maxName(100, 'b');
    EXPECT_NO_THROW(p.setName(maxName));
    EXPECT_EQ(p.getName(), maxName);
}

TEST(PartTest, SetAmountAndCriticalInvalidThrows)
{
    Part p;
    EXPECT_THROW(p.setAmount(-5), std::invalid_argument);
    EXPECT_THROW(p.setCriticalAmount(-2), std::invalid_argument);

    EXPECT_NO_THROW(p.setAmount(0));
    EXPECT_EQ(p.getAmount(), 0);
    EXPECT_NO_THROW(p.setCriticalAmount(0));
    EXPECT_EQ(p.getCriticalAmount(), 0);
}

TEST(PartTest, SetPriceInvalidThrows)
{
    Part p;

    EXPECT_THROW(p.setPrice(-1.0), std::invalid_argument);

    EXPECT_NO_THROW(p.setPrice(0.0));
    EXPECT_DOUBLE_EQ(p.getPrice(), 0.0);
    EXPECT_NO_THROW(p.setPrice(12.345));
    EXPECT_DOUBLE_EQ(p.getPrice(), 12.345);
}

TEST(PartTest, SettersDoNotChangeOnException)
{
    Part p(5, "Nut", 7, 2, 1.5);

    EXPECT_THROW(p.setId(-10), std::invalid_argument);
    EXPECT_EQ(p.getId(), 5);

    EXPECT_THROW(p.setName(""), std::invalid_argument);
    EXPECT_EQ(p.getName(), "Nut");

    EXPECT_THROW(p.setAmount(-3), std::invalid_argument);
    EXPECT_EQ(p.getAmount(), 7);

    EXPECT_THROW(p.setCriticalAmount(-1), std::invalid_argument);
    EXPECT_EQ(p.getCriticalAmount(), 2);

    EXPECT_THROW(p.setPrice(-2.0), std::invalid_argument);
    EXPECT_DOUBLE_EQ(p.getPrice(), 1.5);
}