#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

// Constants
const float R = 6371.0f;  // Radius of Earth in kilometers

// Struct for storing airport data
struct Airport {
    std::string tag;
    float latitude;
    float longitude;
};

// Function to calculate Haversine distance
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

int main() {
    std::ifstream infile("airport_coordinates_trimmed.txt");
    std::ofstream outfile("airport_distances.txt");

    if (!infile.is_open()) {
        std::cerr << "Error opening airport_coordinates_trimmed.txt." << std::endl;
        return 1;
    }

    std::vector<Airport> airports;
    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Airport airport;
        if (iss >> airport.tag >> airport.longitude >> airport.latitude) {
            airports.push_back(airport);
        }
    }
    infile.close();

    int num_airports = airports.size();
    for (int i = 0; i < num_airports; ++i) {
        std::vector<std::pair<float, std::string>> distance_tag_pairs;
        for (int j = 0; j < num_airports; ++j) {
            float distance = haversine_distance(
                airports[i].latitude, airports[i].longitude,
                airports[j].latitude, airports[j].longitude);
            distance_tag_pairs.emplace_back(distance, airports[j].tag);
        }
        std::sort(distance_tag_pairs.begin(), distance_tag_pairs.end());
        outfile << airports[i].tag << ":\n";
        for (int k = 0; k < std::min(250, num_airports); ++k) {
            outfile << distance_tag_pairs[k].second << " " << distance_tag_pairs[k].first << "\n";
        }
        outfile << "END\n";
    }
    outfile.close();

    std::cout << "airport_distances.txt created successfully." << std::endl;
    return 0;
}
