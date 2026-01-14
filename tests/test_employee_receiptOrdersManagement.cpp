#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "../include/common/util/csv_data_manipulator.hpp"
#include "../include/employee/usersManagement.hpp"
#include "../include/employee/devicesManagement.hpp"
#include "../include/employee/receiptOrdersManagement.hpp"

namespace fs = std::filesystem;

class ReceiptOrderManagerTest : public ::testing::Test {
protected:
    UserManager* userManager;
    DeviceManager* deviceManager;
    ReceiptOrderManager* receiptOrderManager;

    void SetUp() override {
        // Ensure data directory exists
        fs::create_directories("./data");
        
        // Create files in correct order: dependencies first
        createTestUsersCSV();
        createTestDevicesCSV();
        createTestReceiptOrdersCSV();

        userManager = new UserManager();
        deviceManager = new DeviceManager();
        receiptOrderManager = new ReceiptOrderManager(*userManager, *deviceManager);
    }

    void TearDown() override {
        delete receiptOrderManager;
        delete deviceManager;
        delete userManager;

        std::remove("./data/receiptOrders.csv");
        std::remove("./data/users.csv");
        std::remove("./data/devices.csv");
        
        // Clean up generated files
        if (fs::exists("./ReceiptOrders")) {
            fs::remove_all("./ReceiptOrders");
        }
    }

    void createTestUsersCSV() {
        std::ofstream file("./data/users.csv");
        file << "id,firstName,lastName,email,phone,pin\n";
        file << "1,Marko,Markovic,marko@test.com,065123456,1234\n";
        file << "2,Petar,Petrovic,petar@test.com,065654321,5678\n";
        file.close();
    }

    void createTestDevicesCSV() {
        std::ofstream file("./data/devices.csv");
        file << "ID,Brand,Model,IMEI,State\n";
        file << "1,Samsung,Galaxy S21,123456789012345,NEW\n";
        file << "2,iPhone,13 Pro,987654321098765,USED\n";
        file << "3,Xiaomi,Redmi Note 10,111222333444555,DAMAGED\n";
        file.close();
    }

    void createTestReceiptOrdersCSV() {
        std::ofstream file("./data/receiptOrders.csv");
        file << "id,user_id,device_imei,description,price,taken\n";
        file << "1,1,123456789012345,Pokvareni ekran,200.00,1\n";
        file << "2,2,987654321098765,Ne puni se,150.00,0\n";
        file << "3,1,111222333444555,Nema zvuka,100.00,1\n";
        file.close();
    }
};

TEST_F(ReceiptOrderManagerTest, SearchForReceiptOrder_ExistingOrder) {
    EXPECT_TRUE(receiptOrderManager->searchForReceiptOrder(1));
    EXPECT_TRUE(receiptOrderManager->searchForReceiptOrder(2));
    EXPECT_TRUE(receiptOrderManager->searchForReceiptOrder(3));
}

TEST_F(ReceiptOrderManagerTest, SearchForReceiptOrder_NonExistingOrder) {
    EXPECT_FALSE(receiptOrderManager->searchForReceiptOrder(9999));
    EXPECT_FALSE(receiptOrderManager->searchForReceiptOrder(0));
    EXPECT_FALSE(receiptOrderManager->searchForReceiptOrder(-1));
}

TEST_F(ReceiptOrderManagerTest, IsReceiptOrderFree_FreeOrder) {
    EXPECT_TRUE(receiptOrderManager->isReceiptOrderFree(1));
    EXPECT_TRUE(receiptOrderManager->isReceiptOrderFree(3));
}

TEST_F(ReceiptOrderManagerTest, IsReceiptOrderFree_TakenOrder) {
    EXPECT_FALSE(receiptOrderManager->isReceiptOrderFree(2));
}

TEST_F(ReceiptOrderManagerTest, IsReceiptOrderFree_NonExistingOrder) {
    EXPECT_FALSE(receiptOrderManager->isReceiptOrderFree(9999));
}

TEST_F(ReceiptOrderManagerTest, ChangeReceiptOrderStatus_Success) {
    // Initial state: free (taken=1)
    EXPECT_TRUE(receiptOrderManager->isReceiptOrderFree(1));

    // Change to not free (taken=0)
    receiptOrderManager->changeReceiptOrderStatus(1, 0);
    EXPECT_FALSE(receiptOrderManager->isReceiptOrderFree(1));

    // Change back to free (taken=1)
    receiptOrderManager->changeReceiptOrderStatus(1, 1);
    EXPECT_TRUE(receiptOrderManager->isReceiptOrderFree(1));
}

TEST_F(ReceiptOrderManagerTest, ChangeReceiptOrderStatus_MultipleChanges) {
    int orderId = 3;

    // Initial: free
    EXPECT_TRUE(receiptOrderManager->isReceiptOrderFree(orderId));

    // Change 1: take
    receiptOrderManager->changeReceiptOrderStatus(orderId, 0);
    EXPECT_FALSE(receiptOrderManager->isReceiptOrderFree(orderId));

    // Change 2: free
    receiptOrderManager->changeReceiptOrderStatus(orderId, 1);
    EXPECT_TRUE(receiptOrderManager->isReceiptOrderFree(orderId));

    // Change 3: take again
    receiptOrderManager->changeReceiptOrderStatus(orderId, 0);
    EXPECT_FALSE(receiptOrderManager->isReceiptOrderFree(orderId));
}

TEST_F(ReceiptOrderManagerTest, ListReceiptOrders_NoException) {
    testing::internal::CaptureStdout();
    EXPECT_NO_THROW(receiptOrderManager->listReceiptOrders());
    testing::internal::GetCapturedStdout();
}

TEST_F(ReceiptOrderManagerTest, ListFreeReceiptOrders_NoException) {
    testing::internal::CaptureStdout();
    EXPECT_NO_THROW(receiptOrderManager->listFreeReceiptOrders());
    testing::internal::GetCapturedStdout();
}

TEST_F(ReceiptOrderManagerTest, GenerateReceiptOrderTXTFile_ExistingOrder) {
    fs::create_directories("./ReceiptOrders");

    EXPECT_NO_THROW(receiptOrderManager->generateReceiptOrderTXTFile(1));

    EXPECT_TRUE(fs::exists("./ReceiptOrders/nalog_1.txt"));
}