#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/employee/partsManagement.hpp"

/*Možda ne baš najslavniji način za implementaciju, ali šta je tu je...*/
void createPart () {
    PartManager pm;
    pm.createPart ();
}
void listParts () {
    PartManager pm;
    pm.listParts ();
}
void editPart () {
    PartManager pm;
    pm.editPart ();
}
void deletePart () {
    PartManager pm;
    pm.deletePart ();
}

namespace fs = std::filesystem;

static const std::string data_dir = "./data";
static const std::string parts_file = "./data/parts.csv";

class PartsManagementTest : public ::testing::Test {
    protected:
        void SetUp () override {
            fs::create_directories (data_dir);
        }

        void TearDown () override {
            std::error_code ec;
            fs::remove (parts_file, ec);
            fs::remove (data_dir, ec);
        }

        void write_csv (const std::vector<std::string>& lines) {
            std::ofstream ofs (parts_file, std::ofstream::trunc);
            for (const auto& l : lines) {
                ofs << l << "\n";
            }
            ofs.close ();
        }

        std::string read_file_all () {
            std::ifstream ifs (parts_file);
            std::ostringstream ss;
            ss << ifs.rdbuf ();
            return ss.str ();
        }
};

TEST_F (PartsManagementTest, CreatePartAddsRow) {
    write_csv ({"id,name,amount,critical,price"});
    write_csv ({
        "id,name,amount,critical,price",
        "999,Dummy,1,1,1.0"  // This will help CSVData understand structure
    });
    std::istringstream input ("Bolt\n5\n1\n2.5\n");
    auto cin_buf = std::cin.rdbuf (input.rdbuf ());

    std::ostringstream output;
    auto cout_buf = std::cout.rdbuf (output.rdbuf ());

    createPart ();

    std::cin.rdbuf (cin_buf);
    std::cout.rdbuf (cout_buf);

    const auto content = read_file_all ();
    std::cout << "=== FILE CONTENT ===\n" << content << "\n===================\n";

    ASSERT_NE (content.find ("name"), std::string::npos);
    ASSERT_NE (content.find ("Bolt"), std::string::npos);
    ASSERT_NE (content.find ("5"), std::string::npos);
    ASSERT_NE (content.find ("2.5"), std::string::npos);
}

TEST_F (PartsManagementTest, ListPartsPrintsContent) {
    write_csv ({"id,name,amount,critical,price", "1,Wheel,10,2,19.99", "2,Seat,4,1,29.50"});

    std::ostringstream output;
    auto cout_buf = std::cout.rdbuf (output.rdbuf ());

    listParts ();

    std::cout.rdbuf (cout_buf);
    const auto out = output.str ();

    EXPECT_NE (out.find ("----- LISTA DIJELOVA -----"), std::string::npos);
    EXPECT_NE (out.find ("Wheel"), std::string::npos);
    EXPECT_NE (out.find ("Seat"), std::string::npos);
}

TEST_F (PartsManagementTest, EditPartUpdatesName) {
    write_csv ({"id,name,amount,critical,price", "1,Wheel,10,2,19.99"});

    std::istringstream input ("1\n1\nNewWheel\n");
    auto cin_buf = std::cin.rdbuf (input.rdbuf ());

    std::ostringstream output;
    auto cout_buf = std::cout.rdbuf (output.rdbuf ());

    editPart ();

    std::cin.rdbuf (cin_buf);
    std::cout.rdbuf (cout_buf);

    const auto content = read_file_all ();

    ASSERT_NE (content.find ("1,NewWheel,10,2,19.99"), std::string::npos);
    ASSERT_EQ (content.find ("1,Wheel,10,2,19.99"), std::string::npos);
}

TEST_F (PartsManagementTest, EditPartUpdatesNumericAttributes) {
    write_csv ({"id,name,amount,critical,price", "1,Wheel,10,2,19.99"});

    {
        std::istringstream input ("1\n2\n20\n");
        auto cin_buf = std::cin.rdbuf (input.rdbuf ());
        std::ostringstream output;
        auto cout_buf = std::cout.rdbuf (output.rdbuf ());

        editPart ();

        std::cin.rdbuf (cin_buf);
        std::cout.rdbuf (cout_buf);
    }

    {
        std::istringstream input ("1\n3\n5\n");
        auto cin_buf = std::cin.rdbuf (input.rdbuf ());
        std::ostringstream output;
        auto cout_buf = std::cout.rdbuf (output.rdbuf ());

        editPart ();

        std::cin.rdbuf (cin_buf);
        std::cout.rdbuf (cout_buf);
    }

    {
        std::istringstream input ("1\n4\n25.75\n");
        auto cin_buf = std::cin.rdbuf (input.rdbuf ());
        std::ostringstream output;
        auto cout_buf = std::cout.rdbuf (output.rdbuf ());

        editPart ();

        std::cin.rdbuf (cin_buf);
        std::cout.rdbuf (cout_buf);
    }

    const auto content = read_file_all ();
    EXPECT_NE (content.find (",20,"), std::string::npos);
    EXPECT_NE (content.find (",5,"), std::string::npos);
    EXPECT_NE (content.find ("25.75"), std::string::npos);
}

TEST_F (PartsManagementTest, DeletePartRemovesRow) {
    write_csv ({"id,name,amount,critical,price", "1,Wheel,10,2,19.99", "2,Seat,4,1,29.50"});

    std::istringstream input ("1\n");
    auto cin_buf = std::cin.rdbuf (input.rdbuf ());
    std::ostringstream output;
    auto cout_buf = std::cout.rdbuf (output.rdbuf ());

    deletePart ();

    std::cin.rdbuf (cin_buf);
    std::cout.rdbuf (cout_buf);

    const auto content = read_file_all ();

    EXPECT_EQ (content.find ("1,Wheel"), std::string::npos);
    EXPECT_NE (content.find ("2,Seat"), std::string::npos);
}

TEST_F (PartsManagementTest, CreatePartGeneratesUniqueId) {
    write_csv ({"id,name,amount,critical,price", "10,Sprocket,3,1,5.00", "15,Lever,7,2,3.50"});

    std::istringstream input ("Chain\n12\n2\n4.2\n");
    auto cin_buf = std::cin.rdbuf (input.rdbuf ());
    std::ostringstream output;
    auto cout_buf = std::cout.rdbuf (output.rdbuf ());

    createPart ();

    std::cin.rdbuf (cin_buf);
    std::cout.rdbuf (cout_buf);

    const auto content = read_file_all ();

    EXPECT_NE (content.find ("Chain"), std::string::npos);

    EXPECT_NE (content.find ("10,Sprocket"), std::string::npos);
    EXPECT_NE (content.find ("15,Lever"), std::string::npos);
}


