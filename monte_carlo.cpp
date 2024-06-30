#include "monte_carlo.h"

std::mutex mtx;
float global_best_distance = std::numeric_limits<float>::max();
std::chrono::high_resolution_clock::time_point start_time;

void monte_carlo_worker(const std::vector<AirportData>& airports, float& best_distance, std::vector<std::string>& best_path, int& paths_evaluated, int runtime_seconds, const std::vector<float>& probabilities) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::discrete_distribution<> d(probabilities.begin(), probabilities.end());

    while (true) {
        auto current_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = current_time - start_time;
        if (elapsed.count() >= runtime_seconds) break;

        std::vector<std::string> path;
        std::unordered_set<std::string> visited;
        const std::string startAirport = "KDKX";
        path.push_back(startAirport);
        visited.insert(startAirport);

        std::string currentAirport = startAirport;
        float total_distance = 0.0;

        while (path.size() < 110 && !airports.empty()) {
            auto it = std::find_if(airports.begin(), airports.end(), [&](const AirportData& a) {
                return a.tag == currentAirport;
            });

            if (it == airports.end()) {
                std::cerr << "Error: Airport " << currentAirport << " not found in the data.\n";
                break;
            }

            std::vector<std::pair<float, std::string>> nearest_airports;
            for (const auto& adj : it->distances) {
                if (visited.find(adj.airport) == visited.end()) {
                    nearest_airports.push_back({adj.distance, adj.airport});
                    if (nearest_airports.size() >= 2) break; // Select from the nearest 2 airports
                }
            }

            if (nearest_airports.empty()) {
                std::cerr << "No unvisited neighbors found for airport " << currentAirport << ".\n";
                break;
            }

            int selected_index = d(gen); // Use probability to select from the nearest 2 airports
            const auto& selected_airport = nearest_airports[selected_index];

            visited.insert(selected_airport.second);
            path.push_back(selected_airport.second);
            currentAirport = selected_airport.second;
            total_distance += selected_airport.first;
        }

        {
            std::lock_guard<std::mutex> lock(mtx);
            paths_evaluated++;

            if (path.size() == 110 && total_distance < best_distance) {
                best_distance = total_distance;
                best_path = path;
            }

            if (best_distance < global_best_distance) {
                global_best_distance = best_distance;
            }

            int percent_complete = static_cast<int>((elapsed.count() / runtime_seconds) * 100);
            std::cout << "Progress: " << percent_complete << "%, Current minimum distance: " << best_distance << " km, Paths evaluated: " << paths_evaluated << "\r";
            std::cout.flush();
        }
    }
}

void monte_carlo_search(int num_threads, const std::vector<float>& probabilities, int runtime_seconds) {
    start_time = std::chrono::high_resolution_clock::now();
    auto airports = readDistances("airport_distances.txt");
    if (airports.empty()) {
        std::cerr << "No airports data found. Exiting program.\n";
        return;
    }

    float best_distance = std::numeric_limits<float>::max();
    std::vector<std::string> best_path;
    int paths_evaluated = 0;

    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(monte_carlo_worker, std::cref(airports), std::ref(best_distance), std::ref(best_path), std::ref(paths_evaluated), runtime_seconds, std::cref(probabilities));
    }

    for (auto& thread : threads) {
        thread.join();
    }

    if (!best_path.empty()) {
        savePathToFile(best_path, best_distance, "airport_list.txt");
    } else {
        std::cerr << "\nUnable to generate a path of 110 airports.\n";
    }
    std::cout << "\n";
}
