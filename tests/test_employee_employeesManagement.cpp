#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "../include/employee/employeesManagement.hpp"

namespace fs = std::filesystem;

class EmployeeManagerTest : public ::testing::Test {
    protected:
        std::string dataDir = "./data";
        std::string csvPath = dataDir + "/employees.csv";

        void writeInitialCsv () {
            fs::create_directories (dataDir);
            std::ofstream ofs (csvPath, std::ios::trunc);

            ofs << "id,first,last,email,phone,pin,role\n";
            ofs << "1,Jocko,Jevtić,jocko@example.com,123456789,1111,RECEPTIONIST\n";
            ofs.close ();
        }

        void TearDown () override {
            std::error_code ec;
            fs::remove (csvPath, ec);
            fs::remove_all (dataDir, ec);
        }
};

static std::string readFileAll (const std::string& path) {
    std::ifstream ifs (path);
    std::stringstream ss;
    ss << ifs.rdbuf ();
    return ss.str ();
}

struct StdioRedirect {
        std::streambuf* oldCin = nullptr;
        std::streambuf* oldCout = nullptr;
        std::istringstream in;
        std::ostringstream out;

        StdioRedirect (const std::string& input) : in (input) {
            oldCin = std::cin.rdbuf (in.rdbuf ());
            oldCout = std::cout.rdbuf (out.rdbuf ());
        }
        ~StdioRedirect () {
            std::cin.rdbuf (oldCin);
            std::cout.rdbuf (oldCout);
        }
};

TEST_F (EmployeeManagerTest, CreateEmployee_CreatesNewEmployee) {
    writeInitialCsv ();
    EmployeeManager mgr;

    std::string input = "Marko\nMarkovic\nmarko@example.com\n987654321\n2\n";
    StdioRedirect redirect (input);

    mgr.createEmployee ();

    std::string content = readFileAll (csvPath);

    EXPECT_NE (content.find ("Marko"), std::string::npos);
    EXPECT_NE (content.find ("TECHNICIAN"), std::string::npos);

    size_t lines = std::count (content.begin (), content.end (), '\n');
    EXPECT_EQ (lines, 3);
}

TEST_F (EmployeeManagerTest, EditEmployee_UpdatesFirstName) {
    writeInitialCsv ();
    EmployeeManager mgr;

    std::string input = "1\n1\nEdited\n";
    StdioRedirect redirect (input);

    mgr.editEmployee ();

    std::string content = readFileAll (csvPath);
    EXPECT_NE (content.find ("Edited"), std::string::npos);

    EXPECT_EQ (content.find ("Jocko"), std::string::npos);
}

TEST_F (EmployeeManagerTest, DeleteEmployee_RemovesEmployee) {
    writeInitialCsv ();
    EmployeeManager mgr;

    std::string input = "1\n";
    StdioRedirect redirect (input);

    mgr.deleteEmployee ();

    std::string content = readFileAll (csvPath);

    size_t lines = std::count (content.begin (), content.end (), '\n');
    EXPECT_EQ (lines, 1);

    EXPECT_EQ (content.find ("Jocko"), std::string::npos);
}

TEST_F (EmployeeManagerTest, SearchForEmployee_FindsAndNotFinds) {
    writeInitialCsv ();
    EmployeeManager mgr;

    EXPECT_TRUE (mgr.searchForEmployee (1));

    EXPECT_FALSE (mgr.searchForEmployee (9999));
}

TEST_F (EmployeeManagerTest, ListEmployees_PrintsHeaderAndRows) {
    writeInitialCsv ();
    EmployeeManager mgr;

    StdioRedirect redirect ("");
    mgr.listEmployees ();

    std::string out = redirect.out.str ();
    EXPECT_NE (out.find ("----- LISTA ZAPOSLENIH -----"), std::string::npos);
    EXPECT_NE (out.find ("Jocko"), std::string::npos);
}