#include "class_def.hpp"
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <numeric>
#include <algorithm>
#include <limits>
#include <cmath>

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
    std::vector<int> localMinDistances(numThreads, INT_MAX); // Local minimums for each thread
    std::vector<std::vector<int>> localMinPaths(numThreads); // Local paths for each thread

    // Calculate total permutations and permutations per thread
    size_t totalPermutations = std::tgamma(numCities);
    size_t permutationsPerThread = totalPermutations / numThreads;

    auto worker = [&](size_t threadIndex, size_t startPermutation, size_t endPermutation) {
        std::vector<int> localPath(cities);
        size_t currentPermutation = 0;

        // Initialize local minimum distance and path for this thread
        int &localMinDistance = localMinDistances[threadIndex];
        std::vector<int> &localMinPath = localMinPaths[threadIndex];

        // Advance to the starting permutation for this thread
        while(currentPermutation < startPermutation && std::next_permutation(localPath.begin() + 1, localPath.end())) {
            ++currentPermutation;
        }

        do {
            if(currentPermutation >= endPermutation) break; // Check if this thread has finished its assigned permutations

            int distance = 0;
            for (size_t i = 0; i < numCities - 1; ++i) {
                distance += Helpers::graph[localPath[i]][localPath[i + 1]];
            }
            distance += Helpers::graph[localPath.back()][localPath.front()]; // Complete the loop

            if (distance < localMinDistance) {
                localMinDistance = distance;
                localMinPath = localPath;
            }
            ++currentPermutation;
        } while (std::next_permutation(localPath.begin() + 1, localPath.end()));
    };

    // Launch threads
    for (size_t i = 0; i < numThreads; ++i) {
        size_t startPermutation = i * permutationsPerThread;
        size_t endPermutation = (i + 1) * permutationsPerThread;
        threads.emplace_back(worker, i, startPermutation, endPermutation);
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Compare local minimums and update global minimum
    for (size_t i = 0; i < numThreads; ++i) {
        if (localMinDistances[i] < minDistance) {
            std::lock_guard<std::mutex> lock(minPathMutex);
            if (localMinDistances[i] < minDistance) {
                minDistance = localMinDistances[i];
                minPath = localMinPaths[i];
            }
        }
    }

    // Final output
    std::cout << "Final minimum distance: " << minDistance.load() << "\nPath: ";
    for (auto city : minPath) {
        std::cout << city << " ";
    }
    std::cout << "\n";
}
