#include "trim_list.h"
#include "calculate_distances.h"
#include "read_distances.h"
#include "depth_search.h"
#include "nearest_neighbor.h"
#include <iostream>

int main() {
    std::cout << "Starting to create the trimmed list of airports...\n";
    if (create_trimmed_list()) {
        std::cout << "Trimmed list created successfully. Now calculating distances...\n";
        create_distances();
        std::cout << "Distances calculated and saved successfully.\n";

        // Load distances
        auto airports = readDistances("airport_distances.txt");
        if (airports.empty()) {
            std::cerr << "No airports data found. Exiting program.\n";
            std::cerr << "\n";
            return 1;
        }

        // Prompt the user for algorithm choice
        int choice;
        std::cout << "Choose an algorithm:\n";
        std::cout << "1. Nearest Neighbor Search\n";
        std::cout << "2. Depth Search\n";
        std::cout << "Enter your choice (1 or 2): ";
        std::cin >> choice;

        if (choice == 1) {
            nearest_neighbor_search();
        } else if (choice == 2) {
            depth_search();
        } else {
            std::cerr << "Invalid choice. Exiting program.\n";
            return 1;
        }

        std::cout << "Process completed. Check 'airport_list.txt' for the results.\n";
    } else {
        std::cout << "Failed to create the trimmed list. Check if the original data file 'airport_coordinates.txt' is present and correctly formatted.\n";
    }
    return 0;
}
