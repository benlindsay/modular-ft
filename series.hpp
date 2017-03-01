// series.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef SERIES_HPP
#define SERIES_HPP

#include <iostream>
#include "globals.hpp"
#include "system.hpp"
#include "input.hpp"

class Series {
 public:
  virtual void init(input::Input_Reader *ir) { };
  virtual void run(void) = 0;
  System *system;
};

class Single_System : public Series {
 public:
  Single_System(input::Input_Reader *ir) { init(ir); };
  void init(input::Input_Reader *ir);
  void run() {
    std::cout << "Running Single Simulation" << std::endl;
    system->simulate();
  };
};

#endif // SERIES_HPP
