#include <iostream>
#include "class_def.hpp"
#include "c_functions.h"

int main(int argc, char* argv[]){
  //Helpers::graph = Helpers::loadGraph("./graph.txt");
  Helpers::graph = Helpers::loadGraph("dj38.tsp");
  Helpers::displayMenu();
  return 0;
}