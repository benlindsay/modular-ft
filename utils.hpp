// utils.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include "globals.hpp"

namespace utils {

  void die(void);
  void die(std::string);
  std::string to_lower(std::string);

}

#endif // UTILS_HPP
