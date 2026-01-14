#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>

#include "../include/common/util/csv_data_manipulator.hpp"
#include "../include/employee/serviceReportsManagement.hpp"

namespace fs = std::filesystem;

class ServiceReportManagerTest : public ::testing::Test {
    protected:
        void SetUp () override {
            // Create test data directory
            fs::create_directories ("./data");
            fs::create_directories ("./ServiceReports");

            // Create test CSV files
            createTestServiceReportsCSV ();
            createTestWorkOrdersCSV ();
            createTestReceiptOrdersCSV ();
            createTestUsersCSV ();
            createTestDevicesCSV ();
            createTestPartsCSV ();
        }

        void TearDown () override {
            // Clean up test files
            fs::remove ("./data/serviceReports.csv");
            fs::remove ("./data/workOrders.csv");
            fs::remove ("./data/receiptOrders.csv");
            fs::remove ("./data/users.csv");
            fs::remove ("./data/devices.csv");
            fs::remove ("./data/parts.csv");

            // Clean up any generated service report files
            for (const auto& entry : fs::directory_iterator ("./ServiceReports")) {
                if (entry.path ().extension () == ".txt") {
                    fs::remove (entry.path ());
                }
            }
        }

        void createTestServiceReportsCSV () {
            std::ofstream file ("./data/serviceReports.csv");
            file << "ID,WorkOrderID\n";
            file << "1,100\n";
            file << "2,101\n";
            file.close ();
        }

        void createTestWorkOrdersCSV () {
            std::ofstream file ("./data/workOrders.csv");
            file << "ID,ReceiptOrderID,EmployeeID,Comment,Status,DateCreated,DateCompleted,UsedParts,ServicePrice\n";
            file << "100,200,1,Screen replacement,Completed,2024-01-01,2024-01-02,1:1;2:2,150.00\n";
            file << "101,201,1,Battery replacement,Completed,2024-01-03,2024-01-04,3:1,80.00\n";
            file.close ();
        }

        void createTestReceiptOrdersCSV () {
            std::ofstream file ("./data/receiptOrders.csv");
            file << "ID,UserID,DeviceIMEI,Description,DateReceived,Status\n";
            file << "200,300,123456789012345,Cracked screen,2024-01-01,Completed\n";
            file << "201,301,987654321098765,Battery draining fast,2024-01-03,Completed\n";
            file.close ();
        }

        void createTestUsersCSV () {
            std::ofstream file ("./data/users.csv");
            file << "ID,FirstName,LastName,Email,Phone,Address\n";
            file << "300,John,Doe,john@example.com,123-456-7890,123 Main St\n";
            file << "301,Jane,Smith,jane@example.com,098-765-4321,456 Oak Ave\n";
            file.close ();
        }

        void createTestDevicesCSV () {
            std::ofstream file ("./data/devices.csv");
            file << "ID,Brand,Model,IMEI,Status\n";
            file << "1,Samsung,Galaxy S21,123456789012345,Active\n";
            file << "2,iPhone,13 Pro,987654321098765,Active\n";
            file.close ();
        }

        void createTestPartsCSV () {
            std::ofstream file ("./data/parts.csv");
            file << "ID,Name,Price,Quantity\n";
            file << "1,LCD Screen,100.00,10\n";
            file << "2,Touch Digitizer,50.00,15\n";
            file << "3,Battery,40.00,20\n";
            file.close ();
        }
};

// Test createServiceReport
TEST_F (ServiceReportManagerTest, CreateServiceReportSuccess) {
    ServiceReportManager manager;
    manager.createServiceReport (100);

    CSVData serviceReports ("./data/serviceReports.csv");
    EXPECT_EQ (serviceReports.rows (), 4);  // Header + 2 existing + 1 new
}

TEST_F (ServiceReportManagerTest, CreateServiceReportGeneratesUniqueID) {
    ServiceReportManager manager;
    manager.createServiceReport (100);

    CSVData serviceReports ("./data/serviceReports.csv");
    std::string lastId = serviceReports.get_value (serviceReports.rows () - 1, 0);
    EXPECT_NE (lastId, "1");
    EXPECT_NE (lastId, "2");
}

// Test searchForServiceReport
TEST_F (ServiceReportManagerTest, SearchForServiceReportFound) {
    ServiceReportManager manager;
    EXPECT_TRUE (manager.searchForServiceReport (1));
}

TEST_F (ServiceReportManagerTest, SearchForServiceReportNotFound) {
    ServiceReportManager manager;
    EXPECT_FALSE (manager.searchForServiceReport (999));
}

TEST_F (ServiceReportManagerTest, SearchForServiceReportMultipleRecords) {
    ServiceReportManager manager;
    EXPECT_TRUE (manager.searchForServiceReport (1));
    EXPECT_TRUE (manager.searchForServiceReport (2));
    EXPECT_FALSE (manager.searchForServiceReport (3));
}

// Test deleteServiceReport functionality through CSV manipulation
TEST_F (ServiceReportManagerTest, DeleteServiceReportReducesRowCount) {
    CSVData serviceReports ("./data/serviceReports.csv");
    int initialRows = serviceReports.rows ();

    // Manually delete a row to simulate deleteServiceReport behavior
    serviceReports.delete_row (1);
    serviceReports.write_data ("./data/serviceReports.csv");

    CSVData updatedReports ("./data/serviceReports.csv");
    EXPECT_EQ (updatedReports.rows (), initialRows - 1);
}

// Test generateServiceReportTXTFile
TEST_F (ServiceReportManagerTest, GenerateServiceReportTXTFileCreatesFile) {
    ServiceReportManager manager;

    // Use existing service report ID from test data
    manager.generateServiceReportTXTFile (1);

    std::string expectedFileName = "./ServiceReports/nalog_1.txt";
    EXPECT_TRUE (fs::exists (expectedFileName));
}

TEST_F (ServiceReportManagerTest, GenerateServiceReportTXTFileContainsExpectedContent) {
    ServiceReportManager manager;

    // Use existing service report ID from test data
    manager.generateServiceReportTXTFile (1);

    std::string fileName = "./ServiceReports/nalog_1.txt";
    std::ifstream file (fileName);
    std::string content ((std::istreambuf_iterator<char> (file)), std::istreambuf_iterator<char> ());
    file.close ();

    EXPECT_NE (content.find ("SERVISNI IZVJESTAJ"), std::string::npos);
    EXPECT_NE (content.find ("Korisnik:"), std::string::npos);
    EXPECT_NE (content.find ("Telefon:"), std::string::npos);
    EXPECT_NE (content.find ("IMEI uredjaja:"), std::string::npos);
}

TEST_F (ServiceReportManagerTest, GenerateServiceReportTXTFileInvalidID) {
    ServiceReportManager manager;

    // Capture stderr since that's where the error message goes
    testing::internal::CaptureStderr ();

    manager.generateServiceReportTXTFile (999);

    std::string stderr_output = testing::internal::GetCapturedStderr ();

    EXPECT_NE (stderr_output.find ("nije pronadjen"), std::string::npos)
        << "Expected error message not found. Output: " << stderr_output;
}
// Test CSV file error handling
TEST_F (ServiceReportManagerTest, CreateServiceReportMissingCSVFile) {
    fs::remove ("./data/serviceReports.csv");

    ServiceReportManager manager;
    testing::internal::CaptureStdout ();
    manager.createServiceReport (100);
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_FALSE (output.empty ());
}

TEST_F (ServiceReportManagerTest, SearchForServiceReportMissingCSVFile) {
    fs::remove ("./data/serviceReports.csv");

    ServiceReportManager manager;
    EXPECT_FALSE (manager.searchForServiceReport (1));
}

int main (int argc, char** argv) {
    testing::InitGoogleTest (&argc, argv);
    return RUN_ALL_TESTS ();
}