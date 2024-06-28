#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <limits>

// Constants
const float R = 6371.0f;  // Radius of Earth in kilometers
const float MAX_DISTANCE = 1000.0f;  // Maximum distance for inclusion
const std::string KDKX = "KDKX";  // Starting airport

// Struct to hold airport information
struct Airport {
    std::string tag;
    float latitude;
    float longitude;
};

// Haversine distance calculation function
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
    // Check if the output files already exist
    std::ifstream check_trim("airport_coordinates_trimmed.txt");
    std::ifstream check_distances("airport_distances.txt");

    if (check_trim.good()) {
        std::cout << "airport_coordinates_trimmed.txt already exists. Skipping creation." << std::endl;
        check_trim.close();
    } else {
        std::ifstream infile("airport_coordinates.txt");
        std::ofstream outfile_trimmed("airport_coordinates_trimmed.txt");

        if (!infile.is_open()) {
            std::cerr << "Error opening airport_coordinates.txt." << std::endl;
            return 1;
        }

        std::string line;
        std::vector<Airport> airports;
        Airport temp;
        float kdkx_lat = 0.0f, kdkx_lon = 0.0f;

        // Read the initial data and find KDKX
        while (std::getline(infile, line)) {
            std::istringstream iss(line);
            if (iss >> temp.tag >> temp.longitude >> temp.latitude) {
                if (temp.tag == KDKX) {
                    kdkx_lat = temp.latitude;
                    kdkx_lon = temp.longitude;
                }
                airports.push_back(temp);
            }
        }
        infile.close();

        // Filter and write to the trimmed file
        for (const auto& airport : airports) {
            float dist = haversine_distance(kdkx_lat, kdkx_lon, airport.latitude, airport.longitude);
            if (dist <= MAX_DISTANCE) {
                outfile_trimmed << airport.tag << " " << airport.longitude << " " << airport.latitude << "\n";
            }
        }
        outfile_trimmed.close();
    }

    if (check_distances.good()) {
        std::cout << "airport_distances.txt already exists. Exiting program to avoid redundancy." << std::endl;
        check_distances.close();
        return 0;
    }

    // Calculate distances between airports within 1000 km
    std::ifstream infile_trimmed("airport_coordinates_trimmed.txt");
    std::ofstream outfile_distances("airport_distances.txt");
    std::vector<Airport> filtered_airports;
    std::string line;

    if (!infile_trimmed.is_open()) {
        std::cerr << "Error opening airport_coordinates_trimmed.txt." << std::endl;
        return 1;
    }

    // Read trimmed data
    while (std::getline(infile_trimmed, line)) {
        std::istringstream iss(line);
        Airport airport;
        if (iss >> airport.tag >> airport.longitude >> airport.latitude) {
            filtered_airports.push_back(airport);
        }
    }
    infile_trimmed.close();

    // Calculate and write distances
    int num_airports = filtered_airports.size();
    for (int i = 0; i < num_airports; ++i) {
        std::vector<std::pair<float, std::string>> distance_tag_pairs;
        for (int j = 0; j < num_airports; ++j) {
            float distance = haversine_distance(
                filtered_airports[i].latitude, filtered_airports[i].longitude,
                filtered_airports[j].latitude, filtered_airports[j].longitude);
            distance_tag_pairs.emplace_back(distance, filtered_airports[j].tag);
        }
        std::sort(distance_tag_pairs.begin(), distance_tag_pairs.end());
        outfile_distances << filtered_airports[i].tag << ":\n";
        for (int k = 0; k < std::min(250, num_airports); ++k) {
            outfile_distances << distance_tag_pairs[k].second << " " << distance_tag_pairs[k].first << "\n";
        }
        outfile_distances << "END\n";
    }
    outfile_distances.close();
    std::cout << "airport_distances.txt created successfully." << std::endl;

    return 0;
}
