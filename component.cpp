// component.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "component.hpp"
#include <iostream>

component::Species::~Species() {
  free(rho);
  free(rho_avg);
  free(rho_hat);
}

void component::Species::allocate_arrays(FFTW_Utils *fftw_utils) {
  fftw_utils->alloc_wrapper(&rho);
  fftw_utils->alloc_wrapper(&rho_avg);
  fftw_utils->alloc_wrapper(&rho_hat);
}
