#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

#include "read_distances.h"
#include "common.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <random>
#include <chrono>
#include <limits>
#include <thread>
#include <mutex>
#include <cmath>
#include <string>

extern float global_best_distance;
extern std::chrono::high_resolution_clock::time_point start_time;

void monte_carlo_search(int num_threads, const std::vector<float>& probabilities, int runtime_seconds);

#endif // MONTE_CARLO_H
