#include <gtest/gtest.h>

#include <stdexcept>

#include "../include/common/models/Worker.hpp"
#include "../include/common/util/Validate.hpp"

TEST (WorkerTest, DefaultConstructorSetsUnknownRole) {
    Worker w;
    EXPECT_EQ (w.getRole (), Role::UNKNOWN);
}

TEST (WorkerTest, ParamConstructorSetsRole) {
    Worker w (1, "John", "Doe", "john@example.com", "123", 1234, Role::TECHNICIAN);
    EXPECT_EQ (w.getRole (), Role::TECHNICIAN);
}

TEST (WorkerTest, SetRoleValidRolesSucceeds) {
    Worker w;
    EXPECT_NO_THROW (w.setRole (Role::RECEPTIONIST));
    EXPECT_EQ (w.getRole (), Role::RECEPTIONIST);

    EXPECT_NO_THROW (w.setRole (Role::TECHNICIAN));
    EXPECT_EQ (w.getRole (), Role::TECHNICIAN);

    EXPECT_NO_THROW (w.setRole (Role::SERVICE_OWNER));
    EXPECT_EQ (w.getRole (), Role::SERVICE_OWNER);
}

TEST (WorkerTest, SetRoleInvalidRoleThrows) {
    Worker w;
    EXPECT_THROW (w.setRole (Role::UNKNOWN), std::invalid_argument);

    Role bogus = static_cast<Role> (-1);
    EXPECT_FALSE (Validate::isValidRole (bogus));
    EXPECT_THROW (w.setRole (bogus), std::invalid_argument);
}

TEST (WorkerTest, isValidRoleRecognizesValidAndInvalid) {
    EXPECT_TRUE (Validate::isValidRole (Role::RECEPTIONIST));
    EXPECT_TRUE (Validate::isValidRole (Role::TECHNICIAN));
    EXPECT_TRUE (Validate::isValidRole (Role::SERVICE_OWNER));

    EXPECT_FALSE (Validate::isValidRole (Role::UNKNOWN));
    EXPECT_FALSE (Validate::isValidRole (static_cast<Role> (999)));
}
