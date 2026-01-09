#pragma once
#include <string>

class DeviceManager {
    public:
        void createDevice ();
        void listDevice ();
        void editDevice ();
        void deleteDevice ();

    private:
        std::string stateToString (int stateInt);
};
