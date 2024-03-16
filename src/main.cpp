#include <iostream>
#include "class_def.hpp"
#include "c_functions.h"
#include <thread>


int main(int argc, char* argv[]){
  //Helpers::graph = Helpers::loadGraph("./graph.txt");
  //Helpers::graph = Helpers::loadGraph("dj38.tsp");
  Helpers::graph = Helpers::loadGraph("tests/gr17.txt");
  Helpers::displayMenu();
  return 0;
}
