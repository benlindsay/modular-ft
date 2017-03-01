// utils.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "utils.hpp"

void utils::die() {
  std::string message = "";
  utils::die(message);
}

void utils::die(std::string message) {
#ifdef MPI
  if ( RANK == 0 ) {
    std::cout << message << std::endl;
    std::cout << "Exiting program." << std::endl;
  }

  MPI_Finalize() ;
#else
  std::cout << message << std::endl;
    std::cout << "Exiting program." << std::endl;
#endif
  exit(1) ;
}

std::string utils::to_lower(std::string str) {
  // transform is from algorithm library
  // See http://stackoverflow.com/questions/313970/how-to-convert-stdstring-to-lower-case
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
  return str;
}
