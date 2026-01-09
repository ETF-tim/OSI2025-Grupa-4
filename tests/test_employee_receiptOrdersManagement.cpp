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