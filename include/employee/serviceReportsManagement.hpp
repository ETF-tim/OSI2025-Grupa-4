#pragma once
#include <string>

class ServiceReportManager {
    public:
        void createServiceReport (int WorkOrderID);
        void listServiceReport ();
        void deleteServiceReport ();
        bool searchForServiceReport (int serviceReportID);
        void generateServiceReportTXTFile (int serviceReportID);

        void mainServiceReportsManager ();
};
