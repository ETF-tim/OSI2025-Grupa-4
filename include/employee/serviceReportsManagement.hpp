#pragma once
#include <string>

class ServiceReportManager {
    public:
        int createServiceReport (int WorkOrderID);
        void listServiceReport ();
        void deleteServiceReport ();
        bool searchForServiceReport (int serviceReportID);
        void generateServiceReportTXTFile (int serviceReportID);

        void mainServiceReportsManager ();
};
