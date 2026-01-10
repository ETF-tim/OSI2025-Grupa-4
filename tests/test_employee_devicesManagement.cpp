/*
NOTE - 9/1/25
Članovi tima su kreirali anti-pattern koji pri kreaciji bilo čega briše heder red.
Zbog toga jedinični testovi sadrže mock podatke, koji se u opštem slučaju ne bi trebali tu nalaziti.
Ukoliko se sjete implementirati GenerateUniqueID na ljepši način, testovi će biti ponovo napisani!

*/

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/employee/devicesManagement.hpp"

static const std::string data_dir = "./data";
static const std::string devices_path = "./data/devices.csv";

static void ensure_data_dir () {
    std::error_code ec;
    std::filesystem::create_directories (data_dir, ec);
}

static void remove_devices_file () {
    std::error_code ec;
    std::filesystem::remove (devices_path, ec);
}

static void write_devices_file (const std::string& content) {
    ensure_data_dir ();
    std::ofstream ofs (devices_path, std::ofstream::trunc);
    ofs << content;
    ofs.close ();
}

static std::vector<std::string> read_file_lines (const std::string& path) {
    std::vector<std::string> lines;
    std::ifstream ifs (path);
    std::string line;
    while (std::getline (ifs, line)) lines.push_back (line);
    return lines;
}

class DevicesManagementTest : public ::testing::Test {
    protected:
        void SetUp () override {
            remove_devices_file ();
            ensure_data_dir ();
        }

        void TearDown () override {
            remove_devices_file ();
        }

        void with_input (const std::string& input, const std::function<void ()>& fn) {
            std::istringstream iss (input);
            auto* cin_bak = std::cin.rdbuf ();
            std::cin.rdbuf (iss.rdbuf ());
            fn ();
            std::cin.rdbuf (cin_bak);
        }

        std::string capture_stdout (const std::function<void ()>& fn) {
            std::ostringstream oss;
            auto* cout_bak = std::cout.rdbuf ();
            std::cout.rdbuf (oss.rdbuf ());
            fn ();
            std::cout.rdbuf (cout_bak);
            return oss.str ();
        }
};

TEST_F (DevicesManagementTest, CreateDeviceStateUsed) {
    write_devices_file (
        "ID,Brand,Model,IMEI,State\n"
        "999,DummyBrand,DummyModel,000000000000000,NEW\n");
    DeviceManager mgr;

    std::string input = "UsedBrand\nUsedModel\n987654321098765\n2\n";
    with_input (input, [&] () {
        std::string out = capture_stdout ([&] () {
            mgr.createDevice ();
        });
        EXPECT_NE (out.find ("Uspjesno kreiran novi uredjaj"), std::string::npos);
    });

    auto lines = read_file_lines (devices_path);
    ASSERT_EQ (lines.size (), 3u);

    std::istringstream rowss (lines[2]);
    std::string id, brand, model, imei, state;
    std::getline (rowss, id, ',');
    std::getline (rowss, brand, ',');
    std::getline (rowss, model, ',');
    std::getline (rowss, imei, ',');
    std::getline (rowss, state, ',');

    EXPECT_EQ (state, "USED");
}

TEST_F (DevicesManagementTest, CreateDeviceAddsRow) {
    write_devices_file (
        "ID,Brand,Model,IMEI,State\n"
        "999,DummyBrand,DummyModel,000000000000000,NEW\n");
    DeviceManager mgr;

    std::string input = "TestBrand\nTestModel\n123456789012345\n1\n";
    with_input (input, [&] () {
        std::string out = capture_stdout ([&] () {
            mgr.createDevice ();
        });

        EXPECT_NE (out.find ("Uspjesno kreiran novi uredjaj"), std::string::npos);
    });

    auto lines = read_file_lines (devices_path);
    ASSERT_EQ (lines.size (), 3u);

    EXPECT_EQ (lines[0], "ID,Brand,Model,IMEI,State");

    std::istringstream rowss (lines[2]);
    std::string id, brand, model, imei, state;
    std::getline (rowss, id, ',');
    std::getline (rowss, brand, ',');
    std::getline (rowss, model, ',');
    std::getline (rowss, imei, ',');
    std::getline (rowss, state, ',');

    EXPECT_FALSE (id.empty ());
    EXPECT_EQ (brand, "TestBrand");
    EXPECT_EQ (model, "TestModel");
    EXPECT_EQ (imei, "123456789012345");
    EXPECT_EQ (state, "NEW");
}

TEST_F (DevicesManagementTest, EditDeviceChangesBrand) {
    write_devices_file (
        "ID,Brand,Model,IMEI,State\n"
        "1,OldBrand,OldModel,111111111111111,USED\n");

    DeviceManager mgr;

    std::string input = "1\n1\nNewBrand\n";
    with_input (input, [&] () {
        std::string out = capture_stdout ([&] () {
            mgr.editDevice ();
        });
        EXPECT_NE (out.find ("Uspjesno uređen uredjaj"), std::string::npos);
    });

    auto lines = read_file_lines (devices_path);
    ASSERT_EQ (lines.size (), 2u);

    std::istringstream rowss (lines[1]);
    std::string id, brand, model, imei, state;
    std::getline (rowss, id, ',');
    std::getline (rowss, brand, ',');
    std::getline (rowss, model, ',');
    std::getline (rowss, imei, ',');
    std::getline (rowss, state, ',');

    EXPECT_EQ (id, "1");
    EXPECT_EQ (brand, "NewBrand");
    EXPECT_EQ (model, "OldModel");
    EXPECT_EQ (imei, "111111111111111");
    EXPECT_EQ (state, "USED");
}

TEST_F (DevicesManagementTest, DeleteDeviceRemovesRow) {
    write_devices_file (
        "ID,Brand,Model,IMEI,State\n"
        "1,BrandA,ModelA,000000000000001,NEW\n"
        "2,BrandB,ModelB,000000000000002,USED\n");

    DeviceManager mgr;

    std::string input = "2\n";
    with_input (input, [&] () {
        std::string out = capture_stdout ([&] () {
            mgr.deleteDevice ();
        });
        EXPECT_NE (out.find ("Uspjesno obrisan uredjaj"), std::string::npos);
    });

    auto lines = read_file_lines (devices_path);
    ASSERT_EQ (lines.size (), 2u);

    std::istringstream rowss (lines[1]);
    std::string id;
    std::getline (rowss, id, ',');
    EXPECT_EQ (id, "1");
}