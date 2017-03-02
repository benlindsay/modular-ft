// array_utils.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef ARRAY_UTILS_HPP
#define ARRAY_UTILS_HPP

#include <complex>
#include "globals.hpp"
#include "system.hpp"

class Array_Utils {
 public:
  Array_Utils(System *system): system(system) {};
  int get_global_index(int local_index);
  int get_global_index(int *n_global);
  int get_local_index(int *n_local);
  void fill_local_index_array(int local_index, int *n_local);
  void fill_global_index_array(int global_index, int *n_global);
  double get_r2_and_fill_r(int local_index, double *r);
  double get_k2_and_fill_k(int local_index, double *k);
  void zero_average(std::complex<double> *array);
  std::complex<double> integ_trapPBC(std::complex<double> *array);
 private:
  System *system;
};

#endif // ARRAY_UTILS_HPP
