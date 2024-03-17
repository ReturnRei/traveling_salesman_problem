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
#include "class_def.hpp"
// We ended up including the entire world here, remove useless imports later 

using std::cout;
using std::format;

std::mutex TspSolver::coutMutex;

int TspSolver::naive_bruteforce() {
    if (Helpers::graph.empty() || Helpers::graph[0].empty()) {
        std::cerr << "Graph is empty or not properly loaded.\n";
        return -1;
    }

    size_t numCities = Helpers::graph.size();
    std::vector<int> cities(numCities);
    std::iota(cities.begin(), cities.end(), 0);

    //int minBlocked = std::numeric_limits<int>::max();
    int minDistance = std::numeric_limits<int>::max();
    std::vector<int> minPath;
    do {
        int currentDistance = 0;
        for (size_t i = 0; i < numCities - 1; ++i) {
            currentDistance += Helpers::graph[cities[i]][cities[i + 1]];
            if (currentDistance >= minDistance) {
  //              int currMinBlocked = i;
        /* Attempted optimization, most likely barely useful in terms of efficiency */
        //         if (currMinBlocked < minBlocked) {
        //         cout << "Blocked loop " << i << " as current distance " << currentDistance << " is greater than min distance " << minDistance << "\n";
        //         minBlocked = i;
        //         }
        //         break;
             }
         }
        currentDistance += Helpers::graph[cities[numCities - 1]][cities[0]];

        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            //cout << "Current minimal distance: " << minDistance << "\n"; 
            minPath = cities;
        }
    } while (std::next_permutation(cities.begin() + 1, cities.end()));

    std::cout << "Minimum distance: " << minDistance << "\nPath: ";
    for (int city : minPath) {
        std::cout << static_cast<char>('A' + city) << " ";
    }
    std::cout << static_cast<char>('A' + minPath.front()) << std::endl;

    return 0;
}

std::atomic<int> TspSolver::minDistance{INT_MAX};
std::vector<int> TspSolver::minPath;
std::mutex TspSolver::minPathMutex;

void TspSolver::naive_bruteforce_multithreaded() {
    size_t numCities = Helpers::graph.size();
    std::vector<int> cities(numCities);
    std::iota(cities.begin(), cities.end(), 0);

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
}



/* Dynamic solver */
int TspSolver::dynamic_solver() {
    std::lock_guard<std::mutex> lock(coutMutex);

    if (Helpers::graph.empty() || Helpers::graph[0].empty()) {
        std::cerr << "Graph is empty or not properly loaded.\n";
        return -1; 
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
    // for (int city : TspSolver::minPath) {
    //     std::cout << static_cast<char>('A' + city) << " ";
    // }
    // std::cout << std::endl;

    return 0;
}