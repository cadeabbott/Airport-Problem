#include "trim_list.h"
#include "calculate_distances.h"
#include "read_distances.h"
#include "depth_search.h"
#include "nearest_neighbor.h"
#include "monte_carlo.h"
#include "grid_search.h"
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
        std::cout << "3. Monte Carlo Search\n";
        std::cout << "4. Grid Search\n";
        std::cout << "Enter your choice (1, 2, 3, or 4): ";
        std::cin >> choice;

        if (choice == 1) {
            std::cout << "Starting nearest neighbor search...\n";
            nearest_neighbor_search();
            std::cout << "Nearest neighbor search completed. Check 'airport_list.txt' for the results.\n";
        } else if (choice == 2) {
            std::cout << "Starting depth search...\n";
            depth_search();
            std::cout << "Depth search completed. Check 'airport_list.txt' for the results.\n";
        } else if (choice == 3) {
            int num_threads;
            std::cout << "Enter number of threads for Monte Carlo search: ";
            std::cin >> num_threads;
            std::vector<float> probabilities = {0.9f, 0.1f}; // Default probabilities for nearest neighbor
            std::cout << "Starting Monte Carlo search with " << num_threads << " threads...\n";
            monte_carlo_search(num_threads, probabilities, 15, "monte_carlo_output.txt"); // 15-second runtime
            std::cout << "Monte Carlo search completed. Check 'airport_list.txt' for the results.\n";
        } else if (choice == 4) {
            int num_threads;
            std::cout << "Enter number of threads for grid search: ";
            std::cin >> num_threads;
            std::cout << "Starting grid search with " << num_threads << " threads...\n";
            grid_search(num_threads);
        } else {
            std::cout << "Invalid choice. Exiting program.\n";
        }
    } else {
        std::cout << "Failed to create the trimmed list. Check if the original data file 'airport_coordinates.txt' is present and correctly formatted.\n";
    }
    return 0;
}
