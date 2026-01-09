#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/common/models/Bill.hpp"
#include "../include/employee/billsManagement.hpp"

namespace fs = std::filesystem;

static const std::string DATA_DIR = "./data";
static const std::string BILLS_CSV = "./data/bills.csv";

static void ensureDataDir () {
    if (!fs::exists (DATA_DIR)) {
        fs::create_directory (DATA_DIR);
    }
}

static void writeCsvLines (const std::vector<std::string>& lines) {
    ensureDataDir ();
    std::ofstream ofs (BILLS_CSV, std::ios::trunc);
    for (const auto& l : lines) {
        ofs << l << "\n";
    }
}

static std::vector<std::string> readCsvLines () {
    std::vector<std::string> lines;
    std::ifstream ifs (BILLS_CSV);
    std::string line;
    while (std::getline (ifs, line)) {
        if (!line.empty ()) lines.push_back (line);
    }
    return lines;
}

TEST (BillManagerTest, PaymentMethodToStringMapsCorrectly) {
    BillManager mgr;
    EXPECT_EQ (mgr.paymentMethodToString (1), "CASH");
    EXPECT_EQ (mgr.paymentMethodToString (2), "CREDIT_CARD");
    EXPECT_EQ (mgr.paymentMethodToString (3), "DIGITAL_WALLET");
    EXPECT_EQ (mgr.paymentMethodToString (999), "UNKNOWN");
}

TEST (BillManagerTest, CreateBillAppendsRow) {
    writeCsvLines ({"ID,WorkOrderID,PaymentMethod,Price"});

    BillManager mgr;

    int workOrderID = 123;
    double price = 45.5;
    mgr.createBill (workOrderID, PaymentMethod::CASH, price);

    auto lines = readCsvLines ();
    ASSERT_GE (lines.size (), 2u);

    std::string last = lines.back ();

    std::vector<std::string> cols;
    std::stringstream ss (last);
    std::string item;
    while (std::getline (ss, item, ',')) cols.push_back (item);

    ASSERT_EQ (cols.size (), 4u);
    EXPECT_EQ (cols[1], std::to_string (workOrderID));
    EXPECT_EQ (cols[2], "CASH");

    double parsedPrice = std::stod (cols[3]);
    EXPECT_NEAR (parsedPrice, price, 1e-6);
}

TEST (BillManagerTest, DeleteBillRemovesRow) {
    writeCsvLines ({"ID,WorkOrderID,PaymentMethod,Price", "5,200,CREDIT_CARD,100.0", "6,201,CASH,50.0"});

    std::istringstream input ("5\n");
    auto* cinBuf = std::cin.rdbuf ();
    std::cin.rdbuf (input.rdbuf ());

    BillManager mgr;
    mgr.deleteBill ();

    std::cin.rdbuf (cinBuf);

    auto lines = readCsvLines ();

    ASSERT_EQ (lines.size (), 2u);

    std::vector<std::string> cols;
    std::stringstream ss (lines[1]);
    std::string item;
    while (std::getline (ss, item, ',')) cols.push_back (item);
    ASSERT_GE (cols.size (), 1u);
    EXPECT_NE (cols[0], "5");
}

TEST (BillManagerTest, SearchForBillFindsExistingAndNotNonExisting) {
    writeCsvLines ({"ID,WorkOrderID,PaymentMethod,Price", "7,300,DIGITAL_WALLET,75.0"});

    BillManager mgr;
    EXPECT_TRUE (mgr.searchForBill (7));
    EXPECT_FALSE (mgr.searchForBill (999));
}

int main (int argc, char** argv) {
    ::testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS ();
}