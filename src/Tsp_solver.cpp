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
#include "class_def.hpp"
// We ended up including the entire world here, remove useless imports later 

using std::cout;
using std::format;

int TspSolver::naive_bruteforce() {
    if (Helpers::graph.empty() || Helpers::graph[0].empty()) {
        std::cerr << "Graph is empty or not properly loaded.\n";
        return -1;
    }

    size_t numCities = Helpers::graph.size();
    std::vector<int> cities(numCities);
    std::iota(cities.begin(), cities.end(), 0);

    int minDistance = std::numeric_limits<int>::max();
    std::vector<int> minPath;
    int minBlocked = std::numeric_limits<int>::max();
    do {
        int currentDistance = 0;
        for (size_t i = 0; i < numCities - 1; ++i) {
            currentDistance += Helpers::graph[cities[i]][cities[i + 1]];
            if (currentDistance >= minDistance) {
                int currMinBlocked = i;
                if (currMinBlocked < minBlocked) {
                cout << "Blocked loop " << i << " as current distance " << currentDistance << " is greater than min distance " << minDistance << "\n";
                minBlocked = i;
                }
                break;
            }
        }
        currentDistance += Helpers::graph[cities[numCities - 1]][cities[0]];

        if (currentDistance < minDistance) {
            minDistance = currentDistance;
            cout << "Current minimal distance: " << minDistance << "\n"; 
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

int TspSolver::second_implementation(){
auto string_to_print {"Class function being called\tNow sleeping\n"}; //Needed to flush stdout buffer
cout << string_to_print;
std::this_thread::sleep_for(std::chrono::seconds(6));
return 0;
}