#include "depth_search.h"
#include "read_distances.h"
#include <iostream>
#include <limits>

void depth_search() {
    auto airports = readDistances("airport_distances.txt");
    if (airports.empty()) {
        std::cerr << "No airports data found. Exiting program.\n";
        return;
    }
    
    std::cout << "Press Enter to exit.\n";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
