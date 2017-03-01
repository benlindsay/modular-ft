// system.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include <vector>
#include <iostream>
#include "globals.hpp"
#include "input.hpp"
#include "fftw_utils.hpp"

class System {
 public:
  System(input::Input_Reader *ir);
  ~System();
  void simulate();
  void energy_outputs();
  void grid_outputs();

  FFTW_Utils *fftw_utils;

  // Box stuff
  double *Lx;
  int *Nx, *NxL;
  double V;
  double *dx;
  int M, ML;
  int iter;
  int max_iter;
  int grid_freq;
  int energy_freq;
  // Components_Group all_components;
};

#endif // SYSTEM_HPP
