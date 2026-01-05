#include <gtest/gtest.h>
#include "../include/common/models/Device.hpp"
#include <limits>
#include <string>

using std::string;

TEST(DeviceTest, DefaultConstructor)
{
    Device d;
    EXPECT_EQ(d.getId(), -1);
    EXPECT_EQ(d.getBrand(), "");
    EXPECT_EQ(d.getModel(), "");
    EXPECT_EQ(d.getIMEI(), "");
    EXPECT_EQ(d.getState(), State::UNKNOWN);
}

TEST(DeviceTest, ParameterizedConstructorAndGetters)
{
    Device d(42, "BrandX", "ModelY", "490154203237518", State::NEW);
    EXPECT_EQ(d.getId(), 42);
    EXPECT_EQ(d.getBrand(), "BrandX");
    EXPECT_EQ(d.getModel(), "ModelY");
    EXPECT_EQ(d.getIMEI(), "490154203237518");
    EXPECT_EQ(d.getState(), State::NEW);
}

TEST(DeviceTest, SettersAcceptValidValues)
{
    Device d;
    EXPECT_NO_THROW(d.setId(0));
    EXPECT_EQ(d.getId(), 0);

    EXPECT_NO_THROW(d.setId(std::numeric_limits<int>::max() - 1));
    EXPECT_EQ(d.getId(), std::numeric_limits<int>::max() - 1);

    string brand50(50, 'b');
    EXPECT_NO_THROW(d.setBrand(brand50));
    EXPECT_EQ(d.getBrand(), brand50);

    string model50(50, 'm');
    EXPECT_NO_THROW(d.setModel(model50));
    EXPECT_EQ(d.getModel(), model50);

    string imei15 = "490154203237518";
    EXPECT_NO_THROW(d.setIMEI(imei15));
    EXPECT_EQ(d.getIMEI(), imei15);

    EXPECT_NO_THROW(d.setState(State::USED));
    EXPECT_EQ(d.getState(), State::USED);
}

TEST(DeviceTest, SettersRejectInvalidValues)
{
    Device d;

    EXPECT_THROW(d.setId(-1), std::invalid_argument);
    EXPECT_THROW(d.setId(std::numeric_limits<int>::max()), std::invalid_argument);

    EXPECT_THROW(d.setBrand(""), std::invalid_argument);
    string brand51(51, 'b');
    EXPECT_THROW(d.setBrand(brand51), std::invalid_argument);

    EXPECT_THROW(d.setModel(""), std::invalid_argument);
    string model51(51, 'm');
    EXPECT_THROW(d.setModel(model51), std::invalid_argument);

    EXPECT_THROW(d.setIMEI(""), std::invalid_argument);
    EXPECT_THROW(d.setIMEI("12345"), std::invalid_argument);
    EXPECT_THROW(d.setIMEI("1234567890123456"), std::invalid_argument);
    EXPECT_THROW(d.setIMEI("12345678901234A"), std::invalid_argument);

    EXPECT_THROW(d.setState(State::UNKNOWN), std::invalid_argument);
}

TEST(DeviceTest, ValidationFunctions)
{
    EXPECT_TRUE(Device::isValidId(0));
    EXPECT_TRUE(Device::isValidId(std::numeric_limits<int>::max() - 1));
    EXPECT_FALSE(Device::isValidId(-5));
    EXPECT_FALSE(Device::isValidId(std::numeric_limits<int>::max()));

    EXPECT_TRUE(Device::isValidBrand("X"));
    EXPECT_TRUE(Device::isValidBrand(string(50, 'x')));
    EXPECT_FALSE(Device::isValidBrand(""));
    EXPECT_FALSE(Device::isValidBrand(string(51, 'x')));

    EXPECT_TRUE(Device::isValidModel("M"));
    EXPECT_TRUE(Device::isValidModel(string(50, 'm')));
    EXPECT_FALSE(Device::isValidModel(""));
    EXPECT_FALSE(Device::isValidModel(string(51, 'm')));

    EXPECT_TRUE(Device::isValidIMEI("490154203237518"));   // Ispravan IMEI
    EXPECT_TRUE(Device::isValidIMEI("000000000000000"));   // Sve nule -> prolazi Luhnov test
    EXPECT_FALSE(Device::isValidIMEI("490154203237517"));  // Ne prolazi test
    EXPECT_FALSE(Device::isValidIMEI("00000000000000"));   // Prekratak IMEI
    EXPECT_FALSE(Device::isValidIMEI("0000000000000000")); // Predug IMEI
    EXPECT_FALSE(Device::isValidIMEI("00000000000000A"));  // Nisu sve cifre -  ne prolazi

    // State
    EXPECT_TRUE(Device::isValidState(State::NEW));
    EXPECT_TRUE(Device::isValidState(State::USED));
    EXPECT_FALSE(Device::isValidState(State::UNKNOWN));
}
