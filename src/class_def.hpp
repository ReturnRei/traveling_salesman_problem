#pragma once

#include <string>
#include <functional>
#include <vector>
#include <chrono>
#include <vector>
#include <atomic>
#include <mutex>

class Helpers {

public:
    static void runTests();
    static void setMemoryGraph(const std::vector<std::vector<int>>& newGraph);
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
    static std::vector<std::vector<int>> loadTxtFile(const std::string& filePath);
};



class TspSolver {

public:
    static std::atomic<int> minDistance;
    static std::vector<int> minPath;
    static std::mutex minPathMutex;
    static std::mutex coutMutex;
    static void naive_bruteforce_multithreaded(); 
    static int naive_bruteforce();
    static int dynamic_solver();
};