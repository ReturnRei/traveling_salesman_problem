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
    static std::vector<std::vector<int>> graph; 
    static std::vector<std::vector<int>> loadGraph(const std::string& filePath = "./graph.txt");
    static void displayMenu();

private:
    struct Option {
        std::string name;
        std::function<void()> action;
    };
    static std::vector<Option> options;
    static std::string formatTime(std::chrono::duration<long, std::ratio<1l, 1l> > duration);
    static void initializeOptions();
    static std::vector<std::vector<int>> loadTxtFile(const std::string& filePath);
    static void validateMatrix(const std::vector<std::vector<int>>& matrix);
    static std::vector<std::string> listTxtFiles(const std::string& directoryPath);
    static void displayMatrix();
    static void promptUserToChooseGraph();
};



class TspSolver {

public:
    static void naive_bruteforce_multithreaded(int* resultPtr = nullptr); 
    static void naive_bruteforce(int* resultPtr = nullptr);
    static void dynamic_solver(int* resultPtr = nullptr);

private:
    static std::atomic<int> minDistance;
    static std::vector<int> minPath;
    static std::mutex minPathMutex;
    static std::mutex coutMutex;
};