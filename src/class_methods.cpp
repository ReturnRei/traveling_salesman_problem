#include <iostream>
#include <format>
#include "class_def.hpp"
#include <iostream>
#include <format>
#include <chrono>
#include <thread>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm> 
#include <numeric>
// We ended up including the entire world here, remove useless imports later 

using std::cout;
using std::format;


/* tsp Implementation here */

int ActualImplementation::naive_bruteforce() {
    if (Helpers::graph.empty() || Helpers::graph[0].empty()) {
        std::cerr << "Graph is empty or not properly loaded.\n";
        return -1;
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

    return 0;
}

int ActualImplementation::second_implementation(){
auto string_to_print {"Class function being called\tNow sleeping"}; //Lulz we already have kind of a race I'm a skiddy
cout << string_to_print;
std::this_thread::sleep_for(std::chrono::seconds(6));
return 0;
}


/* Helpers down here */


std::vector<Helpers::Option> Helpers::options;

void Helpers::initializeOptions() {
    options = {
        {"Display matrix", &Helpers::displayMatrix},
        {"Naive Approach (bruteforce)", &ActualImplementation::naive_bruteforce},
        {"Compute it in another way", &ActualImplementation::second_implementation},
        {"Exit", [](){}}
    };
}

void Helpers::displayMenu() {
    initializeOptions(); 

    bool exitRequested = false;
    while (!exitRequested) {
        cout << "Please select an option:\n";
        for (size_t i = 0; i < options.size(); ++i) {
            cout << i + 1 << ": " << options[i].name << "\n";
        }

        int choice;
        std::cin >> choice;
        --choice;

        if (choice < 0 || choice >= static_cast<int>(options.size())) {
            cout << "Invalid choice. Please try again.\n";
            continue;
        }

        if (options[choice].name == "Exit") {
            exitRequested = true;
        } else {
            auto start = std::chrono::high_resolution_clock::now();
            options[choice].action();
            auto stop = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
            cout << "Execution time: " << formatTime(duration) << "\n";
        }
    }
}
std::string Helpers::formatTime(std::chrono::duration<long, std::ratio<1l, 1l> > duration) {
    auto hours = duration.count() / 3600;
    auto minutes = (duration.count() % 3600) / 60;
    auto seconds = duration.count() % 60;

    return std::format("{} hour(s) {} minute(s) {} second(s)", hours, minutes, seconds);
}


std::vector<std::vector<int>> Helpers::graph;

std::vector<std::vector<int>> Helpers::loadGraph(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        // Directly handling the error within the function
        std::cerr << "File not found: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::vector<int>> matrix;
    int value;
    std::string line;

    try {
        while (std::getline(file, line)) {
            std::vector<int> row;
            std::istringstream iss(line);
            while (iss >> value) {
                row.push_back(value);
            }
            matrix.push_back(row);
        }
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return matrix;
}

void Helpers::displayMatrix() {
    cout << "\nGraph dimensions: " << graph.size() << "x" << graph[0].size() << "\n";
    for (const auto& row : graph) { 
        for (int value : row) {
            cout << value << "\t";
        }
        cout << "\n";
    }
    cout << "\n";
}