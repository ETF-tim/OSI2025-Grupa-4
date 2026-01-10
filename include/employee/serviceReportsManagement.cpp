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
 // Re-add header and new device to CSV data
    serviceReports.add_row (header, 0);  // Re-add header row
    serviceReports.add_row ({std::to_string (tempId),std::to_string (WorkOrderID)});

    //------------------

    // Writing updated data back to CSV file
    serviceReports.write_data ("./data/serviceReports.csv");
    //------------------
    std::cout << "Uspjesno kreiran novi servisni izvjestaj!" << std::endl;

}

void ServiceReportManager::listServiceReport() 
{
       // Opening CSV file
    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno prikazivanje liste servisnih izvjestaja";
        return;
    }  //------------------

    // Print service reports list
    std::cout << "----- LISTA SERVISNIH IZVJESTAJA -----" << std::endl;
    serviceReports.print_csv_data ();
} 

void ServiceReportManager::deleteServiceReport () {
    // Opening CSV file
    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno brisanje servisnog izvjestaja";
        return;
    }  //------------------

    // Print service reports list
    listServiceReport ();
    //------------------

    // Choose serviceReport to delete by ID and check if it exists
    // -> Choose serviceReport to delete by ID
    std::cout << "Unesite ID servisnog izvjestaja koji zelite obrisati: ";
    int deleteId;
    std::cin >> deleteId;
    std::cin.ignore ();  // Clear newline character from input buffer

    // -> Check if it exists
    bool serviceReportFound = false;
    int rowIndex;
    for (rowIndex = 1; rowIndex < serviceReports.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (serviceReports.get_value (rowIndex, 0)) == deleteId) {
            serviceReportFound = true;
            break;
        }
    }
    if (!serviceReportFound) {
        std::cerr << "Servisni izvjestaj sa unesenim ID-em nije pronadjen." << std::endl;
        return;
    }  //------------------

    // Delete serviceReport from CSV data
    serviceReports.delete_row (rowIndex);
    //------------------

    // Writing updated data back to CSV file
    serviceReports.write_data ("./data/serviceReports.csv");
    // Print updated service reports list
    listServiceReport ();
    std::cout << "Uspjesno obrisan servisni izvjestaj!" << std::endl;
}

bool ServiceReportManager::searchForServiceReport (int serviceReportID) {
    // Opening CSV file
    CSVData serviceReports;
    try {
        serviceReports = CSVData ("./data/serviceReports.csv");
    } catch (std::exception& e) {
        std::cout << e.what () << std::endl;
        std::cerr << "Neuspjesno pretrazivanje servisnog izvjestaja";
        return false;
    }  //------------------

    // Find service report by ID
    for (int rowIndex = 1; rowIndex < serviceReports.rows (); rowIndex++) {  // Start from 1 to skip header row
        if (std::stoi (serviceReports.get_value (rowIndex, 0)) == serviceReportID) {
            return true;
        }
    }
    return false;
}