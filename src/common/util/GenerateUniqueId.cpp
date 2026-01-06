#include "../../../include/common/util/GenerateUniqueId.hpp"

#include <iostream>
#include <limits>
#include <map>
#include <stdexcept>

#include "../../../include/common/util/csv_data_manipulator.hpp"

int generateUniqueId (CSVData& csvFile) {
    // for loop from 0 to number of rows in csvFile
    std::map<int, bool> tempMap;
    for (int i = 0; i < csvFile.rows (); i++) {
        int tempId = stoi (csvFile.get_value (i, 0));
        tempMap[tempId] = true;
    }
    // print tempMap for debugging
    for (int i = 0; i < INT_MAX; i++) {
        if (tempMap.find (i) == tempMap.end ()) {
            return i;
        }
    }
    throw std::out_of_range ("Neuspjesno generisanje jedinstvenog ID-a: dostignut maksimalan broj ID-eva");
}