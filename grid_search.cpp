#include "grid_search.h"
#include "monte_carlo.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <limits>

void grid_search(int num_threads) {
    const int runtime_seconds = 15; // Set runtime to 15 seconds
    float best_distance = std::numeric_limits<float>::max();
    std::vector<float> best_probabilities = {1.0f, 0.0f};
    std::vector<std::string> best_path;

    std::ofstream output_file("grid_search_output.txt");
    output_file << "Nearest_Percent,Second_Nearest_Percent,Min_Distance\n";

    for (int nearest_percent = 100; nearest_percent >= 75; --nearest_percent) {
        int second_nearest_percent = 100 - nearest_percent;
        std::vector<float> probabilities = {
            static_cast<float>(nearest_percent) / 100.0f,
            static_cast<float>(second_nearest_percent) / 100.0f
        };

        std::cout << "Running Monte Carlo with probabilities: Nearest " << nearest_percent << "%, Second Nearest " << second_nearest_percent << "%" << std::endl;

        global_best_distance = std::numeric_limits<float>::max();
        monte_carlo_search(num_threads, probabilities, runtime_seconds, "monte_carlo_output.txt");

        float current_distance = global_best_distance;
        if (current_distance < best_distance) {
            best_distance = current_distance;
            best_probabilities = probabilities;

            std::ifstream input_file("monte_carlo_output.txt");
            best_path.clear();
            std::string line;
            while (std::getline(input_file, line)) {
                best_path.push_back(line);
            }
            input_file.close();
        }

        output_file << nearest_percent << "," << second_nearest_percent << "," << current_distance << "\n";
    }

    output_file.close();

    // Save the best path to airport_list.txt
    std::ofstream path_file("airport_list.txt");
    for (const auto& airport : best_path) {
        path_file << airport << "\n";
    }
    path_file << "Total distance: " << best_distance << " km\n";
    path_file.close();

    std::cout << "Grid search completed. Best probabilities: Nearest " << best_probabilities[0] * 100 << "%, Second Nearest " << best_probabilities[1] * 100 << "%, Best distance: " << best_distance << " km\n";
}
