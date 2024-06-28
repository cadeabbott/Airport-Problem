#ifndef READ_DISTANCES_H
#define READ_DISTANCES_H

#include <vector>
#include <string>

struct Distance {
    std::string airport;
    float distance;
};

struct AirportData {
    std::string tag;
    std::vector<Distance> distances;
};

std::vector<AirportData> readDistances(const std::string& filename);

#endif // READ_DISTANCES_H
