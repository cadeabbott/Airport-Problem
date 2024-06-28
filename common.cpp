#include "common.h"

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
