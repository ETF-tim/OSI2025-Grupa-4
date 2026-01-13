#pragma once
#include <string>

class DeviceManager {
    public:
        void createDevice ();
        void listDevice ();
        void editDevice ();
        void deleteDevice ();
        bool searchForDevice (int deviceId);
        std::string getDeviceIMEI (int deviceId);
        void mainDeviceManager();
    private:
        std::string stateToString (int stateInt);
        
};
