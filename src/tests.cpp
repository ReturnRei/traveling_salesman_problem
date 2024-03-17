#include "class_def.hpp"
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>

void Helpers::runTests(){

const std::vector<std::vector<int>> five = {
    {0, 3, 4, 2, 7},
    {3, 0, 4, 6, 3},
    {4, 4, 0, 5, 8},
    {2, 6, 5, 0, 6},
    {7, 3, 8, 6, 0}
};


/* Run tests on one matrix */
setMemoryGraph(five);
try {
    int* minDistance = TspSolver::naive_bruteforce();
    assert(*minDistance == 19);
    std::cout << "Assert for " << *minDistance << " succeded\n";
    //minDistance = nullptr; so this would've leaked!
    delete minDistance;
    minDistance = TspSolver::naive_bruteforce_multithreaded();
    assert(*minDistance == 19);
    std::cout << "Assert for " << *minDistance << " succeded\n";
    delete minDistance;
    minDistance = TspSolver::dynamic_solver();
    assert(*minDistance == 19);
    std::cout << "Assert for " << *minDistance << " succeded\n";
    delete minDistance;
} catch (const std::bad_alloc& e) {
    std::cerr << "Memory allocation failed: " << e.what() << std::endl;
} catch (const std::runtime_error& e) {
    std::cerr << "Runtime error: " << e.what() << std::endl;
} catch (const std::exception& e) {
    std::cerr << "An unexpected error occurred: " << e.what() << std::endl;
}
}