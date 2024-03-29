#include <iostream>
#include <format>
#include <iostream>
#include <format>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <numeric>
#include <atomic>
#include <cmath>
#include "class_def.hpp"
// We ended up including the entire world here, remove useless imports later 
// Now you know that those comments and the world we included will live here for years, people will not not remove it and no one will never know why it's here.
using std::cout; // There is no reason to declare that here
// (again I know it's in one of the exemples, my bad)
// Declare using namespace std; to get all the names that are declared in std
// directly available (no need to write std:: for each function)
using std::format;

std::mutex TspSolver::coutMutex;
std::atomic<int> TspSolver::minDistance{INT_MAX};
std::vector<int> TspSolver::minPath;
std::mutex TspSolver::minPathMutex;

void TspSolver::naive_bruteforce(int* resultPtr) {
    // That could go with the MatrixValidator (or in the Graph class, close by the validate function)
    if (Helpers::graph.empty() || Helpers::graph[0].empty()) {
        std::cerr << "Graph is empty or not properly loaded.\n";
    }
        if (Helpers::graph[0].size() > 15) {
        std::cout << "Cowardly refusing to compute for a day, use size < 16 or use the dynamic approach\n";
        return;
    }
    size_t numCities = Helpers::graph.size();
    std::vector<int> cities(numCities);
    std::iota(cities.begin(), cities.end(), 0);

    int minDistance = std::numeric_limits<int>::max();
    std::vector<int> minPath;
    do {
        int currentDistance = 0;
        for (size_t i = 0; i < numCities - 1; ++i) {
            currentDistance += Helpers::graph[cities[i]][cities[i + 1]];
         }
       currentDistance += Helpers::graph[cities[numCities - 1]][cities[0]];

        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            minPath = cities;
        }
    } while (std::next_permutation(cities.begin() + 1, cities.end()));

    std::cout << "Minimum distance: " << minDistance << "\nPath: ";
    for (int city : minPath) {
        std::cout << static_cast<char>('A' + city) << " ";
    }
    std::cout << static_cast<char>('A' + minPath.front()) << std::endl;
    if (resultPtr != nullptr) {
        *resultPtr = minDistance;
    }
}


void TspSolver::naive_bruteforce_multithreaded(int* resultPtr) {
    // This look like another Graph::validate_size function or somehting
    if (Helpers::graph[0].size() > 15) {
        std::cout << "Cowardly refusing to compute for a day, use size < 16 or use the dynamic approach\n";
        return;
    }
    size_t numCities = Helpers::graph.size();
    std::vector<int> cities(numCities);
    std::iota(cities.begin(), cities.end(), 0);

    // ::getThreads()
    const size_t numThreads = std::thread::hardware_concurrency() - 2;
    std::cout << "Running with threads n: " << numThreads << std::endl;
    std::vector<std::thread> threads;
    std::vector<int> localMinDistances(numThreads, INT_MAX); 
    std::vector<std::vector<int>> localMinPaths(numThreads);

    size_t totalPermutations = std::tgamma(numCities);
    size_t permutationsPerThread = totalPermutations / numThreads;

    auto worker = [&](size_t threadIndex, size_t startPermutation, size_t endPermutation) {
        std::vector<int> localPath(cities);
        size_t currentPermutation = 0;

        int &localMinDistance = localMinDistances[threadIndex];
        std::vector<int> &localMinPath = localMinPaths[threadIndex];

        while(currentPermutation < startPermutation && std::next_permutation(localPath.begin() + 1, localPath.end())) {
            ++currentPermutation;
        }

        do {
            if(currentPermutation >= endPermutation) break; 
            int distance = 0;
            for (size_t i = 0; i < numCities - 1; ++i) {
                distance += Helpers::graph[localPath[i]][localPath[i + 1]];
            }
            distance += Helpers::graph[localPath.back()][localPath.front()];

            if (distance < localMinDistance) {
                localMinDistance = distance;
                localMinPath = localPath;
            }
            ++currentPermutation;
        } while (std::next_permutation(localPath.begin() + 1, localPath.end()));
    };

    for (size_t i = 0; i < numThreads; ++i) {
        size_t startPermutation = i * permutationsPerThread;
        size_t endPermutation = (i + 1) * permutationsPerThread;
        threads.emplace_back(worker, i, startPermutation, endPermutation);
    }

    for (auto& t : threads) {
        t.join();
    }

    for (size_t i = 0; i < numThreads; ++i) {
        if (localMinDistances[i] < minDistance) {
            std::lock_guard<std::mutex> lock(minPathMutex);
            if (localMinDistances[i] < minDistance) {
                minDistance = localMinDistances[i];
                minPath = localMinPaths[i];
            }
        }
    }

    std::cout << "Final minimum distance: " << minDistance.load() << "\nPath: ";
    for (auto city : minPath) {
        std::cout << static_cast<char>('A' + city) << " ";
    }
    std::cout << "\n";
            if (resultPtr != nullptr) {
        *resultPtr = minDistance;
    }
    // Multithreading is hard. Keep exploring, make tests passing memory adresses
    // from a thread to another ecc.
    // Here you would gain a lot of clarity by splitting your code
    // into smaller functions.
}

void TspSolver::dynamic_solver(int* resultPtr) {
    std::lock_guard<std::mutex> lock(coutMutex); //Doubt about this one

    if (Helpers::graph.empty() || Helpers::graph[0].empty()) {
        std::cerr << "Graph is empty or not properly loaded.\n";
        return; 
    }

    size_t numCities = Helpers::graph.size();
    std::vector<std::vector<int>> memo(1 << numCities, std::vector<int>(numCities, -1));
    std::vector<int> parent(numCities, -1);
    std::vector<bool> visited(numCities, false);
    visited[0] = true;

    std::function<int(int, int)> tsp = [&](int pos, int mask) -> int {
        if (mask == (1 << numCities) - 1) {
            return Helpers::graph[pos][0];
        }
        if (memo[mask][pos] != -1) {
            return memo[mask][pos];
        }

        int ans = std::numeric_limits<int>::max();
        for (size_t city = 0; city < numCities; ++city) {
            if (!visited[city] && !(mask & (1 << city))) {
                // Are we really doing this here?
                // 1 << city literally means "take 1 in binary and shift it city times to the left"
                // So yes, it makes 2^city (the absurd number of permutations) but...is this your logic?
                // Isn't it more readable to say std::pow(2, city) ?
                // Also & means bitewise AND (1 & 1 is 1, 1 & 0 is 0, 0 & 1 is 0, and 0 & 0 is 0)
                // It might work, you might understand why it works, I might do too..but.. who wants to debug this the day it stops working?
                visited[city] = true;
                int newAns = Helpers::graph[pos][city] + tsp(city, mask | (1 << city));
                if (newAns < ans) {
                    ans = newAns;
                    parent[pos] = city;
                }
                visited[city] = false;
            }
        }
        memo[mask][pos] = ans;
        return ans;
    };

    TspSolver::minDistance = tsp(0, 1); 


    std::cout << "Minimum distance: " << minDistance.load() << "\n";
    // Path reconstruction not implemented for dynamic

        if (resultPtr != nullptr) {
        *resultPtr = minDistance;
    }
}