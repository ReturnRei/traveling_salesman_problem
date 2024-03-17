#include "class_def.hpp"
#include <vector>
#include <iostream>

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
displayMatrix();
TspSolver::naive_bruteforce();
TspSolver::naive_bruteforce_multithreaded();
TspSolver::dynamic_solver();
// Assert etc 

std::cout << "Ran tests for matrix five";

// Another iteration
Helpers::graph = Helpers::loadGraph("graphs/gr17.txt"); 
displayMatrix();
TspSolver::dynamic_solver();


// Another iteration
Helpers::graph = Helpers::loadGraph("graphs/gr24.txt"); 
displayMatrix();
TspSolver::dynamic_solver();
}