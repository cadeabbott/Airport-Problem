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

        // Prompt the user to select the algorithm
        int choice;
        std::cout << "Select the search algorithm:\n";
        std::cout << "1. Nearest Neighbor\n";
        std::cout << "2. Depth Search\n";
        std::cout << "Enter your choice (1 or 2): ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Starting nearest neighbor search...\n";
            nearest_neighbor_search();
            std::cout << "Nearest neighbor search completed. Check 'airport_list.txt' for the results.\n";
        } else if (choice == 2) {
            size_t maxDepth = 5;
            size_t maxBreadth = 10;
            std::cout << "Enter the maximum depth (default 5): ";
            std::cin >> maxDepth;
            std::cout << "Enter the maximum breadth (default 10): ";
            std::cin >> maxBreadth;

            std::cout << "Starting depth search...\n";
            depth_search(maxDepth, maxBreadth);
            std::cout << "Depth search completed. Check 'airport_list.txt' for the results.\n";
        } else {
            std::cout << "Invalid choice. Exiting program.\n";
        }
    } else {
        std::cout << "Failed to create the trimmed list. Check if the original data file 'airport_coordinates.txt' is present and correctly formatted.\n";
    }
    return 0;
}
