#include <iostream>
#include "class_def.hpp"
#include "c_functions.hpp"

int main(int argc, char* argv[]){
  Helpers::graph = Helpers::loadGraph("./graph.txt");
  Helpers::displayMenu();
  return 0;
}