#include "trim_list.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

bool create_trimmed_list() {
    std::ifstream checkFile("airport_coordinates_trimmed.txt");
    if (checkFile.good()) {
        std::cout << "Trimmed list already exists." << std::endl;
        checkFile.close();
        return true;  // Proceed to distance calculation
    }

    std::ifstream infile("airport_coordinates.txt");
    if (!infile.is_open()) {
        std::cerr << "Error opening airport_coordinates.txt." << std::endl;
        return false;
    }

    std::vector<Airport> airports;
    std::string line;
    Airport kdkx_airport;
    bool kdkx_found = false;

    std::cout << "Starting to read and process airport data..." << std::endl;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        Airport airport;
        if (iss >> airport.tag >> airport.longitude >> airport.latitude) {
            airports.push_back(airport);
            if (airport.tag == KDKX) {
                kdkx_airport = airport;
                kdkx_found = true;
            }
        }
    }
    infile.close();

    if (!kdkx_found) {
        std::cerr << "KDKX not found in data." << std::endl;
        return false;
    }

    std::ofstream outfile("airport_coordinates_trimmed.txt");
    outfile << kdkx_airport.tag << " " << kdkx_airport.longitude << " " << kdkx_airport.latitude << "\n";
    int total_airports = airports.size();
    int processed_count = 0;
    for (const auto& airport : airports) {
        if (airport.tag != KDKX) {
            float dist = haversine_distance(kdkx_airport.latitude, kdkx_airport.longitude, airport.latitude, airport.longitude);
            if (dist <= MAX_DISTANCE) {
                outfile << airport.tag << " " << airport.longitude << " " << airport.latitude << "\n";
            }
        }
        processed_count++;
        int percent_complete = static_cast<int>((processed_count * 100.0) / total_airports);
        std::cout << "Processing progress: " << percent_complete << "%\r";
        std::cout.flush();
    }
    std::cout << std::endl << "Airport list trimmed and saved successfully." << std::endl;
    outfile.close();
    return true;
}
