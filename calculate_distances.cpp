#include "common.h"
#include "calculate_distances.h"

void create_distances() {
    // Check if the output file already exists
    std::ifstream checkFile("airport_distances.txt");
    if (checkFile.good()) {
        std::cout << "Distance list already exists." << std::endl;
        checkFile.close();
        return;  // Exit the function early if the file exists
    }

    std::ifstream infile("airport_coordinates_trimmed.txt");
    if (!infile.is_open()) {
        std::cerr << "Error opening airport_coordinates_trimmed.txt." << std::endl;
        return;
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

    // Create and open the output file
    std::ofstream outfile("airport_distances.txt");

    int num_airports = airports.size();
    std::cout << "Starting distance calculations..." << std::endl;
    for (int i = 0; i < num_airports; ++i) {
        std::vector<std::pair<float, std::string>> distance_tag_pairs;
        for (int j = 0; j < num_airports; ++j) {
            float distance = haversine_distance(
                airports[i].latitude, airports[i].longitude,
                airports[j].latitude, airports[j].longitude);
            distance_tag_pairs.emplace_back(distance, airports[j].tag);
        }
        std::sort(distance_tag_pairs.begin(), distance_tag_pairs.end());

        // Write sorted distances to the output file
        outfile << airports[i].tag << ":\n";
        for (int k = 0; k < std::min(100, num_airports); ++k) {
            outfile << distance_tag_pairs[k].second << " " << distance_tag_pairs[k].first << "\n";
        }
        outfile << "END\n";

        // Output progress
        int percent_complete = static_cast<int>((i + 1) * 100.0 / num_airports);
        std::cout << "Progress: " << percent_complete << "% complete.\r";
        std::cout.flush();
    }
    outfile.close();
    std::cout << std::endl << "Distance calculations completed successfully." << std::endl;
}