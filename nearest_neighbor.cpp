#include "nearest_neighbor.h"
#include "read_distances.h"
#include "common.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

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
                currentAirport = adj.airport;
                total_distance += adj.distance;
                foundNext = true;
                break;
            }
        }

        if (!foundNext) {
            std::cerr << "No unvisited neighbors found for airport " << currentAirport << ".\n";
            break;
        }
    }

    // Save the path to a file
    savePathToFile(path, total_distance, "airport_list.txt");
}
