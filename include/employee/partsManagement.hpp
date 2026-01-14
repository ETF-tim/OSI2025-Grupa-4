#pragma once

class PartManager {
    public:
        void createPart ();
        void listParts ();
        void editPart ();
        void deletePart ();
        bool searchForPart (int partId);
        bool isPartAvailable (int partId, int quantity);
        bool decreasePartStock (int partId, int quantity);
        bool increasePartStock (int partId, int quantity);
        double getPartPrice (int partId);

        void mainPartsManagement ();
};