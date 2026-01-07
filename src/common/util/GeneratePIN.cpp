#include "../../../include/common/util/GeneratePIN.hpp"

#include <cstdlib>
#include <random>

int generatePIN () {
    return rand () % 9000 + 1000;  // Generates a random number between 1000 and 9999
}