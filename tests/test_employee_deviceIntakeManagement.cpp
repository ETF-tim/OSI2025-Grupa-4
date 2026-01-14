#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "../include/employee/deviceIntakeManagement.hpp"
#include "../include/employee/devicesManagement.hpp"
#include "../include/employee/receiptOrdersManagement.hpp"
#include "../include/employee/usersManagement.hpp"

namespace fs = std::filesystem;

struct IoRedirect {
        IoRedirect (std::istream& in, std::ostream& out, const std::string& input)
            : cinbuf (std::cin.rdbuf ()), coutbuf (std::cout.rdbuf ()) {
            inss.str (input);
            std::cin.rdbuf (inss.rdbuf ());
            std::cout.rdbuf (outss.rdbuf ());
        }
        ~IoRedirect () {
            std::cin.rdbuf (cinbuf);
            std::cout.rdbuf (coutbuf);
        }
        std::istringstream inss;
        std::ostringstream outss;
        std::streambuf* cinbuf;
        std::streambuf* coutbuf;
};

class DeviceIntakeManagerTest : public ::testing::Test {
    protected:
        void SetUp () override {
            fs::create_directories ("./data");

            std::ofstream users ("./data/users.csv");
            users << "id,firstName,lastName,email,phone,pin\n";
            users << "1,Existing,User,existing@test.com,1111111111,1111\n";
            users << "999,Dummy,User,dummy@test.com,9999999999,9999\n";
            users.close ();

            std::ofstream devices ("./data/devices.csv");
            devices << "ID,Brand,Model,IMEI,State\n";
            devices << "1,Samsung,Galaxy,111111111111111,USED\n";
            devices << "999,Dummy,Model,000000000000000,NEW\n";
            devices.close ();

            std::ofstream receipts ("./data/receiptOrders.csv");
            receipts << "id,user_id,device_imei,description,price,taken\n";
            receipts.close ();
        }

        void TearDown () override {
            std::error_code ec;
            fs::remove ("./data/users.csv", ec);
            fs::remove ("./data/devices.csv", ec);
            fs::remove ("./data/receiptOrders.csv", ec);
        }
};

TEST_F (DeviceIntakeManagerTest, CallsAllManagersWhenYesLowercase) {
    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);

    IoRedirect io (std::cin, std::cout,
                   "d\n"
                   "John\n"
                   "Doe\n"
                   "john.doe@test.com\n"
                   "1234567890\n"
                   "1234\n"
                   "d\n"
                   "Apple\n"
                   "iPhone\n"
                   "123456789012345\n"
                   "1\n"
                   "2\n"
                   "2\n"
                   "Screen repair\n"
                   "99.99\n");

    DeviceIntakeManager manager (um, dm, rom);

    EXPECT_NO_THROW (manager.intakeDevice ());
}

TEST_F (DeviceIntakeManagerTest, OnlyReceiptWhenNoLowercase) {
    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);

    IoRedirect io (std::cin, std::cout,
                   "n\n"
                   "n\n"
                   "1\n"
                   "1\n"
                   "Battery replacement\n"
                   "49.99\n");

    DeviceIntakeManager manager (um, dm, rom);

    EXPECT_NO_THROW (manager.intakeDevice ());
}

TEST_F (DeviceIntakeManagerTest, UppercaseChoicesAccepted) {
    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);

    IoRedirect io (std::cin, std::cout,
                   "D\n"
                   "Jane\n"
                   "Smith\n"
                   "jane.smith@test.com\n"
                   "9876543210\n"
                   "5678\n"
                   "N\n"
                   "1\n"
                   "2\n"
                   "Water damage repair\n"
                   "150.00\n");

    DeviceIntakeManager manager (um, dm, rom);

    EXPECT_NO_THROW (manager.intakeDevice ());
}

TEST_F (DeviceIntakeManagerTest, HandlesInvalidInputs) {
    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);

    IoRedirect io (std::cin, std::cout,
                   "x\n"
                   "n\n"
                   "y\n"
                   "n\n"
                   "1\n"
                   "1\n"
                   "General checkup\n"
                   "29.99\n");

    DeviceIntakeManager manager (um, dm, rom);

    EXPECT_NO_THROW (manager.intakeDevice ());
}

TEST_F (DeviceIntakeManagerTest, NewUserExistingDevice) {
    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);

    IoRedirect io (std::cin, std::cout,
                   "d\n"
                   "Alice\n"
                   "Brown\n"
                   "alice.brown@test.com\n"
                   "5551234567\n"
                   "9012\n"
                   "n\n"
                   "1\n"
                   "2\n"
                   "Camera repair\n"
                   "75.50\n");

    DeviceIntakeManager manager (um, dm, rom);

    EXPECT_NO_THROW (manager.intakeDevice ());
}

TEST_F (DeviceIntakeManagerTest, ExistingUserNewDevice) {
    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);

    IoRedirect io (std::cin, std::cout,
                   "n\n"
                   "d\n"
                   "Google\n"
                   "Pixel\n"
                   "999999999999999\n"
                   "2\n"
                   "1\n"
                   "2\n"
                   "Software update\n"
                   "0.00\n");

    DeviceIntakeManager manager (um, dm, rom);

    EXPECT_NO_THROW (manager.intakeDevice ());
}