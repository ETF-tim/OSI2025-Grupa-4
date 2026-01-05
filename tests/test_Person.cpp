#include <gtest/gtest.h>
#include "../include/common/models/Person.hpp"
#include <limits>
#include <string>

TEST(PersonTest, DefaultConstructor)
{
    Person p;
    EXPECT_EQ(p.getId(), -1);
    EXPECT_TRUE(p.getFirstName().empty());
    EXPECT_TRUE(p.getLastName().empty());
    EXPECT_TRUE(p.getEmail().empty());
    EXPECT_TRUE(p.getPhone().empty());
    EXPECT_EQ(p.getPin(), -1);
}

TEST(PersonTest, ParameterizedConstructorAndGetters)
{
    Person p(42, "John", "Doe", "john.doe@example.com", "+123-456 789", 1234);
    EXPECT_EQ(p.getId(), 42);
    EXPECT_EQ(p.getFirstName(), "John");
    EXPECT_EQ(p.getLastName(), "Doe");
    EXPECT_EQ(p.getEmail(), "john.doe@example.com");
    EXPECT_EQ(p.getPhone(), "+123-456 789");
    EXPECT_EQ(p.getPin(), 1234);
}

TEST(PersonTest, SetId_ValidAndInvalid)
{
    Person p;
    EXPECT_NO_THROW(p.setId(0));
    EXPECT_EQ(p.getId(), 0);

    int maxAllowed = std::numeric_limits<int>::max() - 1;
    EXPECT_NO_THROW(p.setId(maxAllowed));
    EXPECT_EQ(p.getId(), maxAllowed);

    EXPECT_THROW(p.setId(-1), std::invalid_argument);
    EXPECT_THROW(p.setId(std::numeric_limits<int>::max()), std::invalid_argument);
}

TEST(PersonTest, FirstAndLastName_Validation)
{
    Person p;
    std::string valid = "Alice";
    EXPECT_NO_THROW(p.setFirstName(valid));
    EXPECT_EQ(p.getFirstName(), valid);

    EXPECT_NO_THROW(p.setLastName(valid));
    EXPECT_EQ(p.getLastName(), valid);

    std::string empty = "";
    EXPECT_THROW(p.setFirstName(empty), std::invalid_argument);
    EXPECT_THROW(p.setLastName(empty), std::invalid_argument);

    std::string tooLong(51, 'x');
    EXPECT_THROW(p.setFirstName(tooLong), std::invalid_argument);
    EXPECT_THROW(p.setLastName(tooLong), std::invalid_argument);
}

TEST(PersonTest, EmailValidation_Various)
{
    Person p;
    EXPECT_NO_THROW(p.setEmail("user@domain.com"));
    EXPECT_EQ(p.getEmail(), "user@domain.com");

    EXPECT_NO_THROW(p.setEmail("a@b.cd"));
    EXPECT_EQ(p.getEmail(), "a@b.cd");

    EXPECT_THROW(p.setEmail(""), std::invalid_argument);
    EXPECT_THROW(p.setEmail("nodomain"), std::invalid_argument);
    EXPECT_THROW(p.setEmail("@domain.com"), std::invalid_argument);
    EXPECT_THROW(p.setEmail("user@"), std::invalid_argument);
    EXPECT_THROW(p.setEmail("user.domain.com"), std::invalid_argument);
    EXPECT_THROW(p.setEmail("user@domaincom"), std::invalid_argument);
    EXPECT_THROW(p.setEmail("user@.com"), std::invalid_argument);
    EXPECT_THROW(p.setEmail("user@domain."), std::invalid_argument);

    std::string longEmail(101, 'a');
    longEmail[10] = '@';
    longEmail[20] = '.';
    EXPECT_THROW(p.setEmail(longEmail), std::invalid_argument);
}

TEST(PersonTest, PhoneValidation_Various)
{
    Person p;
    EXPECT_NO_THROW(p.setPhone("1234567890"));
    EXPECT_EQ(p.getPhone(), "1234567890");

    EXPECT_NO_THROW(p.setPhone("+123-456 789"));
    EXPECT_EQ(p.getPhone(), "+123-456 789");

    EXPECT_THROW(p.setPhone(""), std::invalid_argument);
    EXPECT_THROW(p.setPhone("123ABC"), std::invalid_argument);

    std::string longPhone(16, '1');
    EXPECT_THROW(p.setPhone(longPhone), std::invalid_argument);
}

TEST(PersonTest, PinBoundaryChecks)
{
    Person p;
    EXPECT_NO_THROW(p.setPin(1000));
    EXPECT_EQ(p.getPin(), 1000);

    EXPECT_NO_THROW(p.setPin(9999));
    EXPECT_EQ(p.getPin(), 9999);

    EXPECT_THROW(p.setPin(999), std::invalid_argument);
    EXPECT_THROW(p.setPin(10000), std::invalid_argument);
}