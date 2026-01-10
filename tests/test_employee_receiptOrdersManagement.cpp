#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/employee/receiptOrdersManagement.hpp"

static void write_csv (const std::string& path, const std::vector<std::string>& lines) {
    std::filesystem::create_directories (std::filesystem::path (path).parent_path ());
    std::ofstream ofs (path, std::ios::trunc);
    for (const auto& l : lines) {
        ofs << l << "\n";
    }
    ofs.close ();
}

static std::string read_file (const std::string& path) {
    std::ifstream ifs (path);
    std::ostringstream ss;
    ss << ifs.rdbuf ();
    return ss.str ();
}

TEST (ReceiptOrderManagerTest, Search_ReturnsFalseWhenNotFound) {
    const std::string path = "./data/receiptOrders.csv";
    write_csv (path, {"id,user_id,device_imei,description,price", "1,10,IMEI123,Test description,100.0"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    EXPECT_FALSE (mgr.searchForReceiptOrder (2));
}

TEST (ReceiptOrderManagerTest, Search_ReturnsTrueWhenFound) {
    const std::string path = "./data/receiptOrders.csv";
    write_csv (path, {"id,user_id,device_imei,description,price", "1,10,IMEI123,Test description,100.0"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    EXPECT_TRUE (mgr.searchForReceiptOrder (1));
}

TEST (ReceiptOrderManagerTest, List_PrintsSortedById) {
    const std::string path = "./data/receiptOrders.csv";
    write_csv (path, {"id,user_id,device_imei,description,price", "5,10,IMEI5,Desc5,50.0", "2,11,IMEI2,Desc2,20.0",
                      "10,12,IMEI10,Desc10,100.0"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    std::ostringstream captured;
    std::streambuf* oldCout = std::cout.rdbuf (captured.rdbuf ());

    mgr.listReceiptOrders ();

    std::cout.rdbuf (oldCout);
    std::string out = captured.str ();

    auto p2 = out.find ("2");
    auto p5 = out.find ("5", p2 == std::string::npos ? 0 : p2);
    auto p10 = out.find ("10", (p5 == std::string::npos ? 0 : p5));
    ASSERT_NE (p2, std::string::npos);
    ASSERT_NE (p5, std::string::npos);
    ASSERT_NE (p10, std::string::npos);
    EXPECT_LT (p2, p5);
    EXPECT_LT (p5, p10);
}

TEST (ReceiptOrderManagerTest, Delete_RemovesEntry) {
    const std::string path = "./data/receiptOrders.csv";
    write_csv (path, {"id,user_id,device_imei,description,price", "1,10,IMEI1,Desc1,10.0", "2,11,IMEI2,Desc2,20.0"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    std::istringstream input ("1\n");
    std::streambuf* oldCin = std::cin.rdbuf (input.rdbuf ());

    std::ostringstream captured;
    std::streambuf* oldCout = std::cout.rdbuf (captured.rdbuf ());

    mgr.deleteReceiptOrder ();

    std::cin.rdbuf (oldCin);
    std::cout.rdbuf (oldCout);

    std::string final_csv = read_file (path);

    EXPECT_EQ (final_csv.find ("1,10,IMEI1,Desc1,10.0"), std::string::npos);
    EXPECT_NE (final_csv.find ("2,11,IMEI2,Desc2,20.0"), std::string::npos);
}
TEST (ReceiptOrderManagerTest, Add_AppendsNewEntry) {
    const std::string receipt_orders_path = "./data/receiptOrders.csv";
    const std::string users_path = "./data/users.csv";
    const std::string devices_path = "./data/devices.csv";

    write_csv (users_path, {"id,firstName,lastName,email,phone,pin", "13,Test,User,test@example.com,1234567890,1111",
                            "999,Dummy,User,dummy@example.com,0000000000,0000"});

    write_csv (devices_path,
               {"ID,Brand,Model,IMEI,State", "3,TestBrand,TestModel,IMEI3,NEW", "999,DummyBrand,DummyModel,DummyIMEI,NEW"});

    write_csv (receipt_orders_path, {"id,user_id,device_imei,description,price,taken"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    std::istringstream input ("13\n3\nDesc3\n30.0\n");
    std::streambuf* oldCin = std::cin.rdbuf (input.rdbuf ());

    std::ostringstream captured;
    std::streambuf* oldCout = std::cout.rdbuf (captured.rdbuf ());

    mgr.createReceiptOrder ();

    std::cin.rdbuf (oldCin);
    std::cout.rdbuf (oldCout);

    std::string final_csv = read_file (receipt_orders_path);

    EXPECT_NE (final_csv.find ("13"), std::string::npos);
    EXPECT_NE (final_csv.find ("IMEI3"), std::string::npos);
    EXPECT_NE (final_csv.find ("Desc3"), std::string::npos);
    EXPECT_NE (final_csv.find ("30"), std::string::npos);
    EXPECT_NE (final_csv.find ("1"), std::string::npos);

    EXPECT_NE (final_csv.find ("id,user_id,device_imei,description,price,taken"), std::string::npos);
}

TEST (ReceiptOrderManagerTest, ListFree_PrintsOnlyFreeEntries) {
    const std::string path = "./data/receiptOrders.csv";
    write_csv (path, {"id,user_id,device_imei,description,price,taken", "1,10,IMEI1,Desc1,10.0,1", "2,11,IMEI2,Desc2,20.0,0",
                      "3,12,IMEI3,Desc3,30.0,1"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    std::ostringstream captured;
    std::streambuf* oldCout = std::cout.rdbuf (captured.rdbuf ());

    mgr.listFreeReceiptOrders ();

    std::cout.rdbuf (oldCout);
    std::string out = captured.str ();

    EXPECT_NE (out.find ("1 | 10 | IMEI1 | Desc1 | 10.0 | 1"), std::string::npos);
    EXPECT_NE (out.find ("3 | 12 | IMEI3 | Desc3 | 30.0 | 1"), std::string::npos);

    EXPECT_EQ (out.find ("2 | 11 | IMEI2"), std::string::npos);
    EXPECT_EQ (out.find ("IMEI2"), std::string::npos);

    EXPECT_NE (out.find ("LISTA SLOBODNIH PRIJEMNIH NALOGA"), std::string::npos);
    EXPECT_NE (out.find ("=== CSV Data"), std::string::npos);
}
TEST (ReceiptOrderManagerTest, IsReceiptOrderFree_ReturnsCorrectStatus) {
    const std::string path = "./data/receiptOrders.csv";
    write_csv (path, {"id,user_id,device_imei,description,price,taken", "1,10,IMEI1,Desc1,10.0,1", "2,11,IMEI2,Desc2,20.0,0"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    EXPECT_TRUE (mgr.isReceiptOrderFree (1));
    EXPECT_FALSE (mgr.isReceiptOrderFree (2));

    EXPECT_FALSE (mgr.isReceiptOrderFree (999));
}

TEST (ReceiptOrderManagerTest, ChangeReceiptOrderStatus_UpdatesCsvFile) {
    const std::string path = "./data/receiptOrders.csv";
    write_csv (path, {"id,user_id,device_imei,description,price,taken", "1,10,IMEI1,Desc1,10.0,1", "2,11,IMEI2,Desc2,20.0,0"});

    UserManager userManager;
    DeviceManager deviceManager;
    ReceiptOrderManager mgr (userManager, deviceManager);

    mgr.changeReceiptOrderStatus (2, true);

    std::string final_csv = read_file (path);

    EXPECT_NE (final_csv.find ("2,11,IMEI2,Desc2,20.0,1"), std::string::npos);

    EXPECT_NE (final_csv.find ("1,10,IMEI1,Desc1,10.0,1"), std::string::npos);

    mgr.changeReceiptOrderStatus (1, false);
    final_csv = read_file (path);
    EXPECT_NE (final_csv.find ("1,10,IMEI1,Desc1,10.0,0"), std::string::npos);
}
