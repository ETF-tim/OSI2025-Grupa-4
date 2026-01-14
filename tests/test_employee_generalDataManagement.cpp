#include <gtest/gtest.h>

#include <fstream>
#include <sstream>

#include "../include/common/util/csv_data_manipulator.hpp"
#include "../include/employee/generalDataManagement.hpp"

class GeneralDataManagementTest : public ::testing::Test {
    protected:
        std::string testFilePath = "./data/test_generalData.csv";
        std::string originalFilePath = "./data/generalData.csv";
        std::string backupFilePath = "./data/generalData_backup.csv";

        void SetUp () override {
            std::ifstream original (originalFilePath);
            if (original.good ()) {
                std::ofstream backup (backupFilePath);
                backup << original.rdbuf ();
                original.close ();
                backup.close ();
            }

            std::ofstream testFile (originalFilePath);
            testFile << "Naziv,Adresa,Email,Telefon,JIB\n";
            testFile << "Test Servis,Testna Ulica 123,test@servis.ba,+38761234567,1234567890123\n";
            testFile.close ();
        }

        void TearDown () override {
            std::ifstream backup (backupFilePath);
            if (backup.good ()) {
                std::ofstream original (originalFilePath);
                original << backup.rdbuf ();
                backup.close ();
                original.close ();
                std::remove (backupFilePath.c_str ());
            } else {
                std::remove (originalFilePath.c_str ());
            }
        }
};

TEST_F (GeneralDataManagementTest, ListGeneralDataSuccess) {
    testing::internal::CaptureStdout ();
    GeneralDataManagement gdm;
    gdm.listGeneralData ();
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_NE (output.find ("PODACI SERVISA"), std::string::npos);
    EXPECT_NE (output.find ("Test Servis"), std::string::npos);
    EXPECT_NE (output.find ("Testna Ulica 123"), std::string::npos);
    EXPECT_NE (output.find ("test@servis.ba"), std::string::npos);
    EXPECT_NE (output.find ("+38761234567"), std::string::npos);
    EXPECT_NE (output.find ("1234567890123"), std::string::npos);
}

TEST_F (GeneralDataManagementTest, ListGeneralDataFileNotFound) {
    std::remove (originalFilePath.c_str ());
    testing::internal::CaptureStdout ();
    testing::internal::CaptureStderr ();
    GeneralDataManagement gdm;
    gdm.listGeneralData ();
    std::string stdout_output = testing::internal::GetCapturedStdout ();
    std::string stderr_output = testing::internal::GetCapturedStderr ();

    EXPECT_NE (stderr_output.find ("Neuspjesno prikazivanje"), std::string::npos);
}

TEST_F (GeneralDataManagementTest, EditGeneralDataServiceName) {
    std::istringstream input ("1\nNovi Servis\n");
    std::cin.rdbuf (input.rdbuf ());

    testing::internal::CaptureStdout ();
    GeneralDataManagement gdm;
    gdm.editGeneralData ();
    std::string output = testing::internal::GetCapturedStdout ();

    CSVData generalData (originalFilePath);
    EXPECT_EQ (generalData.get_value (1, 0), "Novi Servis");
    EXPECT_NE (output.find ("Uspjesno"), std::string::npos);
}

TEST_F (GeneralDataManagementTest, EditGeneralDataAddress) {
    std::istringstream input ("2\nNova Adresa 456\n");
    std::cin.rdbuf (input.rdbuf ());

    testing::internal::CaptureStdout ();
    GeneralDataManagement gdm;
    gdm.editGeneralData ();
    testing::internal::GetCapturedStdout ();

    CSVData generalData (originalFilePath);
    EXPECT_EQ (generalData.get_value (1, 1), "Nova Adresa 456");
}

TEST_F (GeneralDataManagementTest, EditGeneralDataEmail) {
    std::istringstream input ("3\nnovi@email.ba\n");
    std::cin.rdbuf (input.rdbuf ());

    testing::internal::CaptureStdout ();
    GeneralDataManagement gdm;
    gdm.editGeneralData ();
    testing::internal::GetCapturedStdout ();

    CSVData generalData (originalFilePath);
    EXPECT_EQ (generalData.get_value (1, 2), "novi@email.ba");
}

TEST_F (GeneralDataManagementTest, EditGeneralDataPhone) {
    std::istringstream input ("4\n+38762999888\n");
    std::cin.rdbuf (input.rdbuf ());

    testing::internal::CaptureStdout ();
    GeneralDataManagement gdm;
    gdm.editGeneralData ();
    testing::internal::GetCapturedStdout ();

    CSVData generalData (originalFilePath);
    EXPECT_EQ (generalData.get_value (1, 3), "+38762999888");
}

TEST_F (GeneralDataManagementTest, EditGeneralDataJIB) {
    std::istringstream input ("5\n9876543210987\n");
    std::cin.rdbuf (input.rdbuf ());

    testing::internal::CaptureStdout ();
    GeneralDataManagement gdm;
    gdm.editGeneralData ();
    testing::internal::GetCapturedStdout ();

    CSVData generalData (originalFilePath);
    EXPECT_EQ (generalData.get_value (1, 4), "9876543210987");
}

TEST_F (GeneralDataManagementTest, EditGeneralDataInvalidChoice) {
    std::istringstream input ("0\n6\n1\nValid Servis\n");
    std::cin.rdbuf (input.rdbuf ());

    testing::internal::CaptureStdout ();
    GeneralDataManagement gdm;
    gdm.editGeneralData ();
    std::string output = testing::internal::GetCapturedStdout ();

    EXPECT_NE (output.find ("Unesite broj podatka (1-5)"), std::string::npos);
}

TEST_F (GeneralDataManagementTest, EditGeneralDataFileNotFound) {
    std::remove (originalFilePath.c_str ());

    testing::internal::CaptureStdout ();
    testing::internal::CaptureStderr ();
    GeneralDataManagement gdm;
    gdm.editGeneralData ();
    testing::internal::GetCapturedStdout ();
    std::string stderr_output = testing::internal::GetCapturedStderr ();

    EXPECT_NE (stderr_output.find ("Neuspjesno azuriranje"), std::string::npos);
}