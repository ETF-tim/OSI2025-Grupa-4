#include "../../include/employee/serviceReportsManagement.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../../include/common/models/WorkOrder.hpp"
#include "../../include/common/util/GenerateUniqueId.hpp"
#include "../../include/common/util/Validate.hpp"
#include "../../include/common/util/csv_data_manipulator.hpp"

void ServiceReportManager::createServiceReport(int WorkOrderID)
{
// Opening CSV file

    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog servisnog izvjestaja";
        return;
    }
    
     // Store and remove header row
    std::vector<std::string> header = serviceReports.get_row (0);
    serviceReports.delete_row (0);  // Remove header row
    //------------------

    // Generate unique ID
    int tempId;
    try {
        tempId = generateUniqueId (serviceReports);
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno kreiranje novog uredjaja";
        return;
    }  //------------------

    // Input and validation for attributes

}