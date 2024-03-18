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


/* 
C++ provides enough abstraction so that I didn't really need to think about memory.
Here I am using raw pointers (althought there's the new and delete cpp operators).
It is done on purpose so I could look at what happens in valgrind.
*/

setMemoryGraph(five);
try {
    int* minDistance = new int;
    TspSolver::naive_bruteforce(minDistance);
    assert(*minDistance == 19);
    std::cout << "Assert for " << *minDistance << " succeded\n";
    TspSolver::naive_bruteforce_multithreaded(minDistance);
    assert(*minDistance == 19);
    std::cout << "Assert for " << *minDistance << " succeded\n";
    TspSolver::dynamic_solver(minDistance);
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