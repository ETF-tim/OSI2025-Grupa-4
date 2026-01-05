#include <gtest/gtest.h>
#include "../include/common/models/Worker.hpp"
#include <stdexcept>

TEST(WorkerTest, DefaultConstructorSetsUnknownRole)
{
    Worker w;
    EXPECT_EQ(w.getRole(), Role::UNKNOWN);
}

TEST(WorkerTest, ParamConstructorSetsRole)
{
    Worker w(1, "John", "Doe", "john@example.com", "123", 1234, Role::TECHNICIAN);
    EXPECT_EQ(w.getRole(), Role::TECHNICIAN);
}

TEST(WorkerTest, SetRoleValidRolesSucceeds)
{
    Worker w;
    EXPECT_NO_THROW(w.setRole(Role::RECEPTIONIST));
    EXPECT_EQ(w.getRole(), Role::RECEPTIONIST);

    EXPECT_NO_THROW(w.setRole(Role::TECHNICIAN));
    EXPECT_EQ(w.getRole(), Role::TECHNICIAN);

    EXPECT_NO_THROW(w.setRole(Role::SERVICE_OWNER));
    EXPECT_EQ(w.getRole(), Role::SERVICE_OWNER);
}

TEST(WorkerTest, SetRoleInvalidRoleThrows)
{
    Worker w;
    EXPECT_THROW(w.setRole(Role::UNKNOWN), std::invalid_argument);

    Role bogus = static_cast<Role>(-1);
    EXPECT_FALSE(isValidRole(bogus));
    EXPECT_THROW(w.setRole(bogus), std::invalid_argument);
}

TEST(WorkerTest, IsValidRoleRecognizesValidAndInvalid)
{
    EXPECT_TRUE(isValidRole(Role::RECEPTIONIST));
    EXPECT_TRUE(isValidRole(Role::TECHNICIAN));
    EXPECT_TRUE(isValidRole(Role::SERVICE_OWNER));

    EXPECT_FALSE(isValidRole(Role::UNKNOWN));
    EXPECT_FALSE(isValidRole(static_cast<Role>(999)));
}