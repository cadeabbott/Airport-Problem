#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <vector>
#include <string>

const float R = 6371.0f;
const std::string KDKX = "KDKX";
const float MAX_DISTANCE = 1000.0f;

struct Airport {
    std::string tag;
    float latitude;
    float longitude;
};

float haversine_distance(float lat1, float lon1, float lat2, float lon2);

#endif
