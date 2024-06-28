#include "depth_search.h"
#include "read_distances.h"
#include "common.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

void dfs(const std::string& currentAirport, std::vector<std::string>& path, std::unordered_set<std::string>& visited, const std::vector<AirportData>& airports, size_t maxDepth, size_t maxBreadth, float& total_distance, float currentDistance = 0.0, size_t depth = 0) {
    if (depth == maxDepth || path.size() >= 110) {
        if (path.size() >= 110) {
            savePathToFile(path, total_distance, "airport_list.txt");
        }
        return;
    }

    auto it = std::find_if(airports.begin(), airports.end(), [&](const AirportData& a) {
        return a.tag == currentAirport;
    });

    if (it == airports.end()) {
        std::cerr << "Error: Airport " << currentAirport << " not found in the data. Aborting search at depth " << depth << ".\n";
        return;
    }

    size_t connectionsConsidered = 0;
    for (const auto& adj : it->distances) {
        if (connectionsConsidered >= maxBreadth) break;
        if (visited.find(adj.airport) == visited.end()) {
            visited.insert(adj.airport);
            path.push_back(adj.airport);
            total_distance += adj.distance;
            dfs(adj.airport, path, visited, airports, maxDepth, maxBreadth, total_distance, currentDistance + adj.distance, depth + 1);
            path.pop_back();
            visited.erase(adj.airport);
            total_distance -= adj.distance;
            connectionsConsidered++;
        }
    }
}

void depth_search(size_t maxDepth, size_t maxBreadth) {
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

    float total_distance = 0.0;

    std::cout << "Starting search from " << startAirport << "...\n";
    dfs(startAirport, path, visited, airports, maxDepth, maxBreadth, total_distance);

    if (path.size() >= 110) {
        savePathToFile(path, total_distance, "airport_list.txt");
    } else {
        std::cerr << "Unable to find a complete path of 110 airports.\n";
    }

    std::cout << "Search completed. Check 'airport_list.txt' for the results.\n";
}
