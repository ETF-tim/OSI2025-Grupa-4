#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <map>
#include <string>

#include "../include/common/models/WorkOrder.hpp"
#include "../include/employee/partsManagement.hpp"
#include "../include/employee/receiptOrdersManagement.hpp"
#include "../include/employee/workOrdersManagement.hpp"

namespace fs = std::filesystem;

static void ensure_data_dir () {
    fs::path data_dir = fs::path ("./data");
    if (!fs::exists (data_dir)) {
        fs::create_directory (data_dir);
    }
}

static void write_file (const std::string& path, const std::string& contents) {
    std::ofstream ofs (path, std::ofstream::trunc);
    ofs << contents;
    ofs.close ();
}

static void remove_file (const std::string& path) {
    std::error_code ec;
    fs::remove (path, ec);
}

static void create_workOrders_csv (const std::vector<std::string>& rows) {
    ensure_data_dir ();
    std::string path = "./data/workOrders.csv";
    std::string contents;
    for (const auto& r : rows) {
        contents += r + "\n";
    }
    write_file (path, contents);
}

TEST (WorkOrderManager_CSV, SearchForWorkOrder_FindsExistingAndNotFound) {
    create_workOrders_csv ({"id,receiptOrderId,status,comment,startDate,endDate,technicianId,parts,servicePrice",
                            "42,10,IN_DIAGNOSTICS,comment,0,0,3,,0", "100,11,COMPLETED,done,0,0,4,1:2,150"});

    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);
    PartManager pm;
    WorkOrderManager wom (rom, pm);

    EXPECT_TRUE (wom.searchForWorkOrder (42));
    EXPECT_TRUE (wom.searchForWorkOrder (100));
    EXPECT_FALSE (wom.searchForWorkOrder (7));

    remove_file ("./data/workOrders.csv");
}

TEST (WorkOrderManager_CSV, IsWorkOrderCompleted_ReturnsCorrectStatus) {
    create_workOrders_csv ({"id,receiptOrderId,status,comment,startDate,endDate,technicianId,parts,servicePrice",
                            "1,5,COMPLETED,ok,0,0,2,1:1,50", "2,6,IN_REPAIR,working,0,0,3,,0"});

    UserManager um;
    DeviceManager dm;
    ReceiptOrderManager rom (um, dm);
    PartManager pm;
    WorkOrderManager wom (rom, pm);

    EXPECT_TRUE (wom.isWorkOrderCompleted (1));
    EXPECT_FALSE (wom.isWorkOrderCompleted (2));
    EXPECT_FALSE (wom.isWorkOrderCompleted (999));

    remove_file ("./data/workOrders.csv");
}

class CleanDataDir : public ::testing::Environment {
    public:
        void TearDown () override {
            std::error_code ec;
            remove_file ("./data/workOrders.csv");
            remove_file ("./data/receiptOrders.csv");
            remove_file ("./data/users.csv");

            fs::path data_dir = fs::path ("./data");
            fs::remove (data_dir, ec);
        }
};

int main (int argc, char** argv) {
    ::testing::InitGoogleTest (&argc, argv);
    ::testing::AddGlobalTestEnvironment (new CleanDataDir ());
    return RUN_ALL_TESTS ();
}