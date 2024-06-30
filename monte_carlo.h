#ifndef MONTE_CARLO_H
#define MONTE_CARLO_H

#include <chrono>
#include <vector>
#include <string>

extern float global_best_distance;
extern std::chrono::high_resolution_clock::time_point start_time;

void monte_carlo_search(int num_threads, const std::vector<float>& probabilities, int runtime_seconds);

#endif // MONTE_CARLO_H
