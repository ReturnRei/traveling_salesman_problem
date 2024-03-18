#include <iostream>
#include <format>
#include <chrono>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <filesystem>
#include <filesystem>
#include <string>
#include "class_def.hpp"

using std::cout;
using std::format;

std::vector<Helpers::Option> Helpers::options;

void Helpers::initializeOptions() {
    options = {
        /*
        Best I could come up with in cpp so that I could use pointers as a POC for the tests and to have a menu that allows both functions with input and without.
        */
        {"Display matrix", &Helpers::displayMatrix},
        {"Single threaded bruteforce solver", [](){TspSolver::naive_bruteforce(nullptr);} },
        {"Multithreaded bruteforce solver", [](){TspSolver::naive_bruteforce_multithreaded(nullptr);}},
        {"Dynamic solver", [](){TspSolver::dynamic_solver(nullptr);}},
        {"Run tests", &Helpers::runTests},
        {"Chose another graph", &Helpers::promptUserToChooseGraph},
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
        } else if (options[choice].name.find("solver") != std::string::npos){
            auto start = std::chrono::high_resolution_clock::now();
            options[choice].action();
            auto stop = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
            cout << "Execution time: " << formatTime(duration) << "\n";
        }
        else {
            options[choice].action();
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
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);

    if (extension == ".txt") {
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
    validateMatrix(matrix);
    return matrix;
}

std::vector<std::string> Helpers::listTxtFiles(const std::string& directoryPath) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
        if (entry.path().extension() == ".txt") {
            files.push_back(entry.path().filename().string());
        }
    }
    return files;
}

void Helpers::promptUserToChooseGraph() {
    const std::string directoryPath = "./graphs";
    auto files = Helpers::listTxtFiles(directoryPath);

    if (files.empty()) {
        std::cerr << "No .txt files found in the directory: " << directoryPath << std::endl;
        return;
    }

    std::cout << "Please select a graph to load:\n";
    for (size_t i = 0; i < files.size(); ++i) {
        std::cout << i + 1 << ": " << files[i] << "\n";
    }

    std::cout << "Enter your choice (number): ";
    size_t choice;
    std::cin >> choice;
    
    if (choice < 1 || choice > files.size()) {
        std::cerr << "Invalid choice. Please try again.\n";
        return;
    }

    std::string selectedFile = directoryPath + "/" + files[choice - 1];
    Helpers::graph = Helpers::loadGraph(selectedFile);
}

void Helpers::setMemoryGraph(const std::vector<std::vector<int>>& newGraph) {
    validateMatrix(newGraph);
    graph = newGraph;
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

void Helpers::validateMatrix(const std::vector<std::vector<int>>& matrix) {
    size_t numRows = matrix.size();

    if (numRows == 0) {
        throw std::invalid_argument("Matrix is empty.");
    }

    for (const auto& row : matrix) {
        if (row.size() != numRows) {
            throw std::invalid_argument("Matrix is not square.");
        }
    }

    for (size_t i = 0; i < numRows; ++i) {
        if (matrix[i][i] != 0) {
            throw std::invalid_argument("Diagonal is not made of zeroes.");
        }
    }
}