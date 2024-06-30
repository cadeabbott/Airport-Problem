#include "read_distances.h"

// Utility function to trim whitespace from the end of a string
std::string rtrim(const std::string& s) {
    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::vector<AirportData> readDistances(const std::string& filename) {
    std::vector<AirportData> airports;
    std::ifstream file(filename);
    std::string line;

    AirportData current_airport;
    bool reading_distances = false;

    while (getline(file, line)) {
        line = rtrim(line);
        if (line.empty()) continue;  // Skip empty lines

        if (line.back() == ':') {
            // Save the previous airport before starting a new one
            if (reading_distances) {
                airports.push_back(current_airport);
                // std::cout << "Saved airport: " << current_airport.tag << " with " << current_airport.distances.size() << " connections." << std::endl;
                reading_distances = false;
            }

            // Start a new airport
            current_airport.tag = line.substr(0, line.size() - 1);
            current_airport.distances.clear();
            // std::cout << "Starting new airport: " << current_airport.tag << std::endl;
            reading_distances = true;
        } else if (line == "END") {
            // End of current airport distances
            if (!current_airport.tag.empty()) {
                airports.push_back(current_airport);
                // std::cout << "Saved airport: " << current_airport.tag << " with " << current_airport.distances.size() << " connections." << std::endl;
                reading_distances = false;
            }
        } else {
            // Add connections to the current airport
            std::istringstream iss(line);
            std::string connectedAirport;
            float distance;
            iss >> connectedAirport >> distance;

            if (!current_airport.tag.empty() && !iss.fail()) {
                current_airport.distances.push_back({connectedAirport, distance});
            } else {
                std::cerr << "Parsing error or no current airport defined in line: " << line << std::endl;
            }
        }
    }

    // Make sure to add the last airport if the file didn't end with "END"
    if (reading_distances && !current_airport.tag.empty()) {
        airports.push_back(current_airport);
        // std::cout << "Saved airport: " << current_airport.tag << " with " << current_airport.distances.size() << " connections." << std::endl;
    }

    file.close();
    return airports;
}
