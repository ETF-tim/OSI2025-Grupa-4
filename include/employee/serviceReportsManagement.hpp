#pragma once
#include <string>

class ServiceReportManager {
    public:
        void createServiceReport (int WorkOrderID);
        void listServiceReport ();
        void deleteServiceReport ();
        bool searchForServiceReport (int serviceReportID);
        void mainServiceReportsManager();
};
