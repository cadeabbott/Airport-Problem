#ifndef READ_DISTANCES_H
#define READ_DISTANCES_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>

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
