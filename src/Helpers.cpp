#include <iostream>
#include <format>
#include <chrono>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <filesystem>
#include "class_def.hpp"

using std::cout;
using std::format;

std::vector<Helpers::Option> Helpers::options;

void Helpers::initializeOptions() {
    options = {
        {"Display matrix", &Helpers::displayMatrix},
        {"Naive Approach (bruteforce)", &TspSolver::naive_bruteforce},
        {"Compute it in another way", &TspSolver::second_implementation},
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

std::string Helpers::formatTime(std::chrono::duration<long, std::ratio<1l, 1l>> duration) {
    auto hours = duration.count() / 3600;
    auto minutes = (duration.count() % 3600) / 60;
    auto seconds = duration.count() % 60;

    return std::format("{} hour(s) {} minute(s) {} second(s)", hours, minutes, seconds);
}

std::vector<std::vector<int>> Helpers::graph;

std::vector<std::vector<int>> Helpers::loadGraph(const std::string& filePath) {
    std::string extension = std::filesystem::path(filePath).extension().string();
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // Ensure extension is lowercase

    if (extension == ".tsp") {
        return loadTspFile(filePath);
    } else if (extension == ".txt") {
        return loadTxtFile(filePath);
    } else {
        std::cerr << "Unsupported file format: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }
}

std::vector<std::vector<int>> Helpers::loadTxtFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "File not found: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::vector<int>> matrix;
    std::string line;
    int value;

    while (std::getline(file, line)) {
        std::vector<int> row;
        std::istringstream iss(line);
        while (iss >> value) {
            row.push_back(value);
        }
        matrix.push_back(row);
    }

    return matrix;
}

std::vector<std::vector<int>> Helpers::loadTspFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        std::cerr << "File not found: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<std::pair<float, float>> coordinates; // Store node coordinates
    std::string line;
    bool nodeSection = false;

    while (std::getline(file, line)) {
        if (line.find("NODE_COORD_SECTION") != std::string::npos) {
            nodeSection = true;
            continue;
        }

        if (nodeSection) {
            if (line.find("EOF") != std::string::npos) break;
            std::istringstream iss(line);
            int id;
            float x, y;
            if (!(iss >> id >> x >> y)) break; // Handle parsing failure
            coordinates.emplace_back(x, y);
        }
    }

    int numNodes = coordinates.size();
    std::vector<std::vector<int>> matrix(numNodes, std::vector<int>(numNodes));

    for (int i = 0; i < numNodes; ++i) {
        for (int j = 0; j < numNodes; ++j) {
            if (i == j) {
                matrix[i][j] = 0;
            } else {
                matrix[i][j] = calculateDistance(coordinates[i].first, coordinates[i].second, coordinates[j].first, coordinates[j].second);
            }
        }
    }

    return matrix;
}

int Helpers::calculateDistance(float x1, float y1, float x2, float y2) {
    return static_cast<int>(std::round(std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2))));
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
