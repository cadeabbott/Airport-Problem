#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

// Radius of Earth in kilometers
const float R = 6371.0f;

// Haversine distance calculation
float haversine_distance(float lat1, float lon1, float lat2, float lon2) {
    const float DEG_TO_RAD = 3.14159265358979323846f / 180.0f;
    lat1 *= DEG_TO_RAD;
    lon1 *= DEG_TO_RAD;
    lat2 *= DEG_TO_RAD;
    lon2 *= DEG_TO_RAD;

    float dlat = lat2 - lat1;
    float dlon = lon2 - lon1;

    float a = std::sin(dlat / 2.0f) * std::sin(dlat / 2.0f) +
              std::cos(lat1) * std::cos(lat2) * std::sin(dlon / 2.0f) * std::sin(dlon / 2.0f);
    float c = 2.0f * std::atan2(std::sqrt(a), std::sqrt(1.0f - a));

    return R * c;
}

struct Airport {
    std::string tag;
    float latitude;
    float longitude;
};

int main() {
    std::ifstream infile("output.txt");
    std::ofstream outfile("distance_matrix.txt");

    if (!infile.is_open() || !outfile.is_open()) {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::vector<Airport> airports;
    std::string line;

    // Read the airport data from output.txt
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Airport airport;
        iss >> airport.tag >> airport.longitude >> airport.latitude;
        airports.push_back(airport);
    }

    infile.close();

    int num_airports = airports.size();

    // Calculate pairwise distances and store in matrix
    std::vector<std::vector<float>> distance_matrix(num_airports, std::vector<float>(num_airports));

    for (int i = 0; i < num_airports; ++i) {
        for (int j = 0; j < num_airports; ++j) {
            distance_matrix[i][j] = haversine_distance(
                airports[i].latitude, airports[i].longitude,
                airports[j].latitude, airports[j].longitude);
        }
    }

    // Write sorted distances to file, only the nearest 250 airports
    for (int i = 0; i < num_airports; ++i) {
        // Create a vector of pairs (distance, tag) for sorting
        std::vector<std::pair<float, std::string>> distance_tag_pairs;

        for (int j = 0; j < num_airports; ++j) {
            distance_tag_pairs.emplace_back(distance_matrix[i][j], airports[j].tag);
        }

        // Sort the pairs by distance
        std::sort(distance_tag_pairs.begin(), distance_tag_pairs.end());

        // Write the sorted distances and corresponding tags to the file, only top 250
        outfile << airports[i].tag << ":\n";
        for (int k = 0; k < std::min(250, num_airports); ++k) {
            outfile << distance_tag_pairs[k].second << " " << distance_tag_pairs[k].first << "\n";
        }
        outfile << "END\n";
    }

    outfile.close();
    std::cout << "Processing complete." << std::endl;

    return 0;
}
