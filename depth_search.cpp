#include "depth_search.h"
#include "read_distances.h"
#include "common.h"
#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <limits>
#include <set>
#include <chrono>

// Helper function to find the MST using Prim's algorithm with a Fibonacci heap
std::vector<std::pair<std::string, std::string>> constructMST(const std::vector<AirportData>& airports) {
    std::vector<std::pair<std::string, std::string>> mst;
    std::unordered_map<std::string, bool> inMST;
    std::priority_queue<std::tuple<float, std::string, std::string>, std::vector<std::tuple<float, std::string, std::string>>, std::greater<>> pq;
    std::string start = "KDKX";

    std::cout << "Constructing the Minimum Spanning Tree (MST)...\n";
    pq.push({0, start, ""});
    size_t count = 0;
    while (!pq.empty()) {
        auto [weight, u, parent] = pq.top();
        pq.pop();

        if (inMST[u]) continue;
        inMST[u] = true;

        if (!parent.empty()) {
            mst.push_back({u, parent});
        }

        for (const auto& airport : airports) {
            if (airport.tag == u) {
                for (const auto& edge : airport.distances) {
                    if (!inMST[edge.airport]) {
                        pq.push({edge.distance, edge.airport, u});
                    }
                }
                break;
            }
        }
        count++;
        if (count % 100 == 0) {
            std::cout << "Progress: " << count << " airports processed in MST construction...\n";
        }
    }
    std::cout << "MST construction completed with " << mst.size() << " edges.\n";
    return mst;
}

// Helper function to find odd degree vertices
std::vector<std::string> findOddDegreeVertices(const std::vector<std::pair<std::string, std::string>>& mst) {
    std::unordered_map<std::string, int> degreeCount;
    for (const auto& edge : mst) {
        degreeCount[edge.first]++;
        degreeCount[edge.second]++;
    }

    std::vector<std::string> oddDegreeVertices;
    for (const auto& [vertex, degree] : degreeCount) {
        if (degree % 2 == 1) {
            oddDegreeVertices.push_back(vertex);
        }
    }
    std::cout << "Found " << oddDegreeVertices.size() << " odd-degree vertices.\n";
    return oddDegreeVertices;
}

// Helper function to find minimum weight perfect matching using a greedy approach
std::vector<std::pair<std::string, std::string>> findMinimumWeightPerfectMatching(const std::vector<std::string>& oddVertices, const std::vector<AirportData>& airports) {
    std::vector<std::pair<std::string, std::string>> matching;
    std::set<std::string> unmatched(oddVertices.begin(), oddVertices.end());

    std::cout << "Finding minimum weight perfect matching...\n";
    size_t count = 0;
    while (!unmatched.empty()) {
        auto it1 = unmatched.begin();
        std::string u = *it1;
        unmatched.erase(it1);

        float minWeight = std::numeric_limits<float>::max();
        std::string minVertex;

        for (const auto& v : unmatched) {
            float weight = std::numeric_limits<float>::max();
            for (const auto& airport : airports) {
                if (airport.tag == u) {
                    for (const auto& edge : airport.distances) {
                        if (edge.airport == v) {
                            weight = edge.distance;
                            break;
                        }
                    }
                    break;
                }
            }
            if (weight < minWeight) {
                minWeight = weight;
                minVertex = v;
            }
        }

        unmatched.erase(minVertex);
        matching.push_back({u, minVertex});
        count++;
        if (count % 10 == 0) {
            std::cout << "Progress: " << count << " pairs matched...\n";
        }
    }

    std::cout << "Minimum weight perfect matching found with " << matching.size() << " edges.\n";
    return matching;
}

// Helper function to create Eulerian Circuit from MST and matching
std::vector<std::pair<std::string, std::string>> createEulerianCircuit(const std::vector<std::pair<std::string, std::string>>& mst, const std::vector<std::pair<std::string, std::string>>& matching) {
    std::vector<std::pair<std::string, std::string>> eulerianCircuit = mst;
    eulerianCircuit.insert(eulerianCircuit.end(), matching.begin(), matching.end());
    std::cout << "Eulerian circuit created with " << eulerianCircuit.size() << " edges.\n";
    return eulerianCircuit;
}

// Helper function to convert Eulerian Circuit to Hamiltonian Circuit
std::vector<std::string> createHamiltonianCircuit(const std::vector<std::pair<std::string, std::string>>& eulerianCircuit, const std::string& startAirport) {
    std::unordered_map<std::string, std::vector<std::string>> adjList;
    for (const auto& edge : eulerianCircuit) {
        adjList[edge.first].push_back(edge.second);
        adjList[edge.second].push_back(edge.first);
    }

    std::unordered_set<std::string> visited;
    std::vector<std::string> path;
    std::string current = startAirport;

    std::cout << "Converting Eulerian circuit to Hamiltonian circuit...\n";
    size_t count = 0;
    while (visited.size() < 110 && !current.empty()) {
        path.push_back(current);
        visited.insert(current);

        for (const auto& neighbor : adjList[current]) {
            if (visited.find(neighbor) == visited.end()) {
                current = neighbor;
                break;
            }
        }
        count++;
        if (count % 10 == 0) {
            std::cout << "Progress: " << count << " airports added to Hamiltonian circuit...\n";
        }
    }

    std::cout << "Hamiltonian circuit created with " << path.size() << " airports.\n";
    return path;
}

void depth_search(size_t maxDepth, size_t maxBreadth) {
    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Reading distances from file...\n";
    auto airports = readDistances("airport_distances.txt");
    if (airports.empty()) {
        std::cerr << "No airports data found. Exiting program.\n";
        return;
    }

    std::cout << "Constructing MST...\n";
    std::vector<std::pair<std::string, std::string>> mst = constructMST(airports);
    std::cout << "Finding odd degree vertices...\n";
    std::vector<std::string> oddDegreeVertices = findOddDegreeVertices(mst);
    std::cout << "Finding minimum weight perfect matching...\n";
    std::vector<std::pair<std::string, std::string>> matching = findMinimumWeightPerfectMatching(oddDegreeVertices, airports);
    std::cout << "Creating Eulerian circuit...\n";
    std::vector<std::pair<std::string, std::string>> eulerianCircuit = createEulerianCircuit(mst, matching);
    std::cout << "Creating Hamiltonian circuit...\n";
    std::vector<std::string> hamiltonianCircuit = createHamiltonianCircuit(eulerianCircuit, "KDKX");

    float total_distance = 0.0;
    for (size_t i = 1; i < hamiltonianCircuit.size(); ++i) {
        auto it = std::find_if(airports.begin(), airports.end(), [&](const AirportData& a) {
            return a.tag == hamiltonianCircuit[i - 1];
        });

        for (const auto& dist : it->distances) {
            if (dist.airport == hamiltonianCircuit[i]) {
                total_distance += dist.distance;
                break;
            }
        }
    }

    if (!hamiltonianCircuit.empty() && hamiltonianCircuit.size() == 110) {
        savePathToFile(hamiltonianCircuit, total_distance, "airport_list.txt");
        std::cout << "Path of 110 airports found and saved to airport_list.txt\n";
    } else {
        std::cerr << "Unable to generate a path of 110 airports.\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Depth search completed in " << duration.count() << " seconds.\n";
}
