#include "monte_carlo.h"
#include "trim_list.h"
#include "calculate_distances.h"
#include "read_distances.h"

int main() {
    std::cout << "Trimming airport list...\n";
    if (create_trimmed_list()) {
        std::cout << "Calculating distances...\n";
        create_distances();

        // Prompt the user for runtime, nearest airport probability, and number of threads
        int runtime_seconds;
        float nearest_prob;
        int num_threads;

        std::cout << "Enter runtime for Monte Carlo search (seconds): ";
        std::cin >> runtime_seconds;

        std::cout << "Enter probability for the nearest airport (0.0 to 1.0): ";
        std::cin >> nearest_prob;

        std::cout << "Enter number of threads for Monte Carlo search: ";
        std::cin >> num_threads;

        // Set probabilities for the nearest and second nearest airports
        std::vector<float> probabilities = {nearest_prob, 1.0f - nearest_prob};

        std::cout << "Starting Monte Carlo search with " << num_threads << " threads...\n";
        monte_carlo_search(num_threads, probabilities, runtime_seconds);
        std::cout << "Monte Carlo search completed!\n";
        std::cout << "Check 'airport_list.txt' for the results!\n";
    } else {
        std::cout << "Failed to create the trimmed list. Check if the original data file 'airport_coordinates.txt' is present and correctly formatted.\n";
    }
    return 0;
}
