#pragma once

#include <string>
#include <functional>
#include <vector>
#include <chrono>

class Helpers {

public:
    static std::vector<std::vector<int>> graph; // 2D Vector, C++ syntax is quite barbaric IMO
    static std::vector<std::vector<int>> loadGraph(const std::string& filePath = "./graph.txt"); // So thats the default one
    static void displayMenu();
    static void displayMatrix();

private:
    struct Option {
        std::string name;
        std::function<void()> action;
    };
    static std::vector<Option> options;
    static std::string formatTime(std::chrono::duration<long, std::ratio<1l, 1l> > duration);
    static void initializeOptions();
    static std::vector<std::vector<int>> loadTxtFile(const std::string& filePath); // For loading .txt files
    static std::vector<std::vector<int>> loadTspFile(const std::string& filePath); // For loading .tsp files
    static int calculateDistance(float x1, float y1, float x2, float y2); // Helper for calculating distances
};



class TspSolver {

public:
    static int naive_bruteforce();
    static int second_implementation();
};