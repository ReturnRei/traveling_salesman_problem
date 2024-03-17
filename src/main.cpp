#include <iostream>
#include "class_def.hpp"
#include "c_functions.h"
#include <thread>


int main(int argc, char* argv[]){
  Helpers::graph = Helpers::loadGraph("graphs/fri26.txt");
  Helpers::displayMenu();
  return 0;
}
