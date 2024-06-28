#include "nearest_neighbor.h"
#include "read_distances.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <fstream>

// Function to save the path and total distance to a file
void savePathToFile(const std::vector<std::string>& path, float total_distance, const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error opening file " << filename << " for writing.\n";
        return;
    }
    for (const std::string& airport : path) {
        outFile << airport << "\n";
    }
    outFile << "Total distance: " << total_distance << " km\n";
    outFile.close();
    std::cout << "Path and total distance saved to " << filename << std::endl;
}

// Function to find the nearest neighbor path
void nearest_neighbor_search() {
    auto airports = readDistances("airport_distances.txt");
    if (airports.empty()) {
        std::cerr << "No airports data found. Exiting program.\n";
        return;
    }

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

        bool foundNext = false;
        for (const auto& adj : it->distances) {
            if (visited.find(adj.airport) == visited.end()) {
                visited.insert(adj.airport);
                path.push_back(adj.airport);
                total_distance += adj.distance;
                currentAirport = adj.airport;
                foundNext = true;
                break;
            }
        }

        if (!foundNext) {
            std::cerr << "No unvisited neighbors found for airport " << currentAirport << ".\n";
            break;
        }
    }

    // Save the path and total distance to a file
    savePathToFile(path, total_distance, "airport_list.txt");
}
