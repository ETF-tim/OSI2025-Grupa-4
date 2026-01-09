#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "../include/employee/usersManagement.hpp"

namespace fs = std::filesystem;

static std::vector<std::vector<std::string>> read_csv (const std::string& path) {
    std::vector<std::vector<std::string>> rows;
    std::ifstream in (path);
    if (!in.is_open ()) return rows;
    std::string line;
    while (std::getline (in, line)) {
        std::vector<std::string> cols;
        std::stringstream ss (line);
        std::string cell;
        while (std::getline (ss, cell, ',')) cols.push_back (cell);
        rows.push_back (cols);
    }
    return rows;
}

class UsersManagementTest : public ::testing::Test {
    protected:
        const fs::path data_dir = "./data";
        const fs::path users_file = data_dir / "users.csv";

        void SetUp () override {
            fs::create_directories (data_dir);

            std::ofstream out (users_file, std::ios::trunc);
            out << "id,firstName,lastName,email,phone,pin\n";
            out << "1,John,Doe,john@example.com,1234567890,1111\n";
            out << "2,Jane,Smith,jane@example.com,0987654321,2222\n";
            out.close ();
        }

        void TearDown () override {
            std::error_code ec;
            fs::remove (users_file, ec);
            fs::remove (data_dir, ec);
        }

        std::string capture_stdout (std::function<void ()> fn) {
            std::streambuf* old_buf = std::cout.rdbuf ();
            std::ostringstream capture;
            std::cout.rdbuf (capture.rdbuf ());
            fn ();
            std::cout.rdbuf (old_buf);
            return capture.str ();
        }

        void provide_stdin_and_run (const std::string& input, std::function<void ()> fn) {
            std::streambuf* old_in = std::cin.rdbuf ();
            std::istringstream in (input);
            std::cin.rdbuf (in.rdbuf ());
            fn ();
            std::cin.rdbuf (old_in);
        }
};

TEST_F (UsersManagementTest, ListUsers_PrintsHeaderAndUsers) {
    UserManager mgr;
    std::string output = capture_stdout ([&mgr] () {
        mgr.listUsers ();
    });

    EXPECT_NE (output.find ("LISTA KORISNIKA"), std::string::npos);
    EXPECT_NE (output.find ("John"), std::string::npos);
    EXPECT_NE (output.find ("Jane"), std::string::npos);
    EXPECT_NE (output.find ("john@example.com"), std::string::npos);
    EXPECT_NE (output.find ("jane@example.com"), std::string::npos);
}

TEST_F (UsersManagementTest, EditUser_UpdatesEmail) {
    UserManager mgr;
    std::string simulated_input = "1\n3\nnewjohn@example.com\n";
    provide_stdin_and_run (simulated_input, [&mgr] () {
        mgr.editUser ();
    });

    auto rows = read_csv (users_file.string ());
    bool found = false;
    for (const auto& row : rows) {
        if (!row.empty () && row[0] == "1") {
            ASSERT_GE (row.size (), 5u);
            EXPECT_EQ (row[3], "newjohn@example.com");
            found = true;
        }
    }
    EXPECT_TRUE (found);
}

TEST_F (UsersManagementTest, DeleteUser_RemovesRow) {
    std::string simulated_input = "2\n";
    UserManager mgr;

    provide_stdin_and_run (simulated_input, [&mgr] () {
        mgr.deleteUser ();
    });

    auto rows = read_csv (users_file.string ());
    bool found = false;
    for (const auto& row : rows) {
        if (!row.empty () && row[0] == "2") {
            found = true;
        }
    }
    EXPECT_FALSE (found);
}