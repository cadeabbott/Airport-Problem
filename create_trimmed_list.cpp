#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>

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
    std::ifstream infile("airport_coordinates.txt");
    std::ofstream outfile("airport_coordinates_trimmed.txt");

    if (!infile.is_open()) {
        std::cerr << "Error opening airport_coordinates.txt for reading." << std::endl;
        return 1;
    }

    std::string line;
    std::vector<Airport> airports;
    Airport temp, kdkx_airport;
    bool kdkx_found = false;

    // Read the initial data and filter based on distance
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (iss >> temp.tag >> temp.longitude >> temp.latitude) {
            airports.push_back(temp);
            if (temp.tag == KDKX) {
                kdkx_airport = temp;  // Save KDKX data
                kdkx_found = true;
            }
        }
    }
    infile.close();

    if (!kdkx_found) {
        std::cerr << "KDKX airport not found in the provided data." << std::endl;
        return 1;
    }

    // Always include KDKX
    outfile << kdkx_airport.tag << " " << kdkx_airport.longitude << " " << kdkx_airport.latitude << "\n";

    // Write to the trimmed file if within 1000 km of KDKX
    for (const auto& airport : airports) {
        if (airport.tag != KDKX) {
            float dist = haversine_distance(kdkx_airport.latitude, kdkx_airport.longitude, airport.latitude, airport.longitude);
            if (dist <= MAX_DISTANCE) {
                outfile << airport.tag << " " << airport.longitude << " " << airport.latitude << "\n";
            }
        }
    }
    outfile.close();

    std::cout << "airport_coordinates_trimmed.txt created successfully." << std::endl;
    return 0;
}
