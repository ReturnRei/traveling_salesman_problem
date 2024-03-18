#include <iostream>
#include "class_def.hpp"

int main(int argc, char* argv[]){

  const std::vector<std::vector<int>> five = {
    {0, 3, 4, 2, 7},
    {3, 0, 4, 6, 3},
    {4, 4, 0, 5, 8},
    {2, 6, 5, 0, 6},
    {7, 3, 8, 6, 0}
};

    try {
      /* In the release project there is no graph.txt in the root dir.
        This is done on purpose to have a try/catch block and allow the user to insert a custom graph. 
      */
        Helpers::graph = Helpers::loadGraph("./graph.txt"); 
        std::cout << "Successfully loaded primary graph." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load primary graph: " << e.what() << std::endl;
        std::cout << "Loading fallback graph." << std::endl;
        Helpers::setMemoryGraph(five);
    }

  Helpers::displayMenu();
  return 0;
}
