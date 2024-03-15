#pragma once

#include <string>
#include <functional>
#include <vector>
#include <chrono>

class Helpers {
/*
This class loads the graph provides a menu to the user, checks time taken for efficiency and calls different functions of the actual implementation
Cette classe load le graphe donne un menu a l'utilisateur, regarde le temps qu'une fonction a pris et appelles les differentes fonctions de l'implementation du solver
*/

public:
    static std::vector<std::vector<int>> graph; // 2D Vector, C++ syntax is quite barbaric IMO
    static std::vector<std::vector<int>> loadGraph(const std::string& filePath = "./graph.txt");
    static void displayMenu();
    static void displayMatrix();

private:
    struct Option {
        std::string name;
        std::function<void()> action;
    };
    static std::vector<Option> options;
    static std::string formatTime(std::chrono::seconds duration);
    static void initializeOptions();

};


class ActualImplementation {
// Class will go through renaming
/*
Here will the actual tsp implementation go 
Ici il y a l'implementation, ou au moins la declaration des fonctions pour compute tsp
*/

    int cval = 4; // This is completely useless was just a test to see compability with C 
public:
    static int naive_bruteforce();
    static int second_implementation();
};