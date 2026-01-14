#pragma once
#include <string>

class DeviceRepairStatusManager {
    public:
        void checkRepairStatus (int userID);

    private:
        std::string formatTimestamp (const std::string& timestampStr);
};