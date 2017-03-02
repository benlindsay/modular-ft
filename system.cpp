// system.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "system.hpp"

System::System(input::Input_Reader *ir) {
  Lx = new double[DIM];
  Nx = new int[DIM];
  NxL = new int[DIM];
  dx = new double[DIM];
  z_local_0 = 0;
  V = 1.0;
  M = 1;
  for (int i = 0; i < DIM; i++) {
    Lx[i] = ir->lx_struct.values[i];
    V *= Lx[i];
    Nx[i] = ir->nx_struct.values[i];
    M *= Nx[i];
    dx[i] = Lx[i] / double(Nx[i]);
  }

  fftw_utils = new FFTW_Utils(ir, this);

  max_iter = ir->max_iter_struct.values[0];
  grid_freq = ir->output_freq_struct.grid_struct.values[0];
  energy_freq = ir->output_freq_struct.energy_struct.values[0];
}

System::~System() {
  delete Lx;
  delete Nx;
}

void System::simulate() {
  std::cout << "Running simulate() method" << std::endl;
  iter = 0;
  energy_outputs();
  grid_outputs();
  for (iter = 1; iter <= max_iter; iter++) {
    if (iter % energy_freq == 0) {
      energy_outputs();
    }
    if (iter % grid_freq == 0) {
      grid_outputs();
    }
  }
}

void System::energy_outputs() {
  std::cout << "energy" << iter << std::endl;
}

void System::grid_outputs() {
  std::cout << "grid" << iter << std::endl;
}
