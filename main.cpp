#include "trim_list.h"
#include "calculate_distances.h"
#include <iostream>

int main() {
    std::cout << "Starting to create the trimmed list of airports...\n";
    if (create_trimmed_list()) {
        std::cout << "Trimmed list created successfully. Now calculating distances...\n";
        create_distances();
        std::cout << "Distances calculated and saved successfully.\n";
    } else {
        std::cout << "Failed to create the trimmed list. Check if the original data file 'airport_coordinates.txt' is present and correctly formatted.\n";
    }
    return 0;
}
