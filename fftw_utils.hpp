// fftw_utils.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef FFTW_UTILS_HPP
#define FFTW_UTILS_HPP

#include <complex>
#include <iostream>
#include "globals.hpp"
#include "input.hpp"

#ifdef MPI
#include "mpi.h"
#include "fftw3-mpi.h"
#else
#include "fftw3.h"
#endif

// Forward declarations of classes used in FFTW_Utils but not defined yet
class System;
namespace input {
  class Input_Reader;
}

class FFTW_Utils {
 public:
  FFTW_Utils(input::Input_Reader *ir, System *system):
    memory_alloced(0), system(system) { init(ir); };
  ~FFTW_Utils();
  void init(input::Input_Reader *ir);

  void alloc_wrapper(std::complex<double>  **arr);
  void alloc_wrapper_fftw_complex(fftw_complex **arr);

  void fft_fwd_wrapper(std::complex<double> *in, std::complex<double> *out);
  void fft_bck_wrapper(std::complex<double> *in, std::complex<double> *out);

  long long memory_alloced;
  System *system;
  int z_start;
  fftw_plan fwd_plan, bck_plan;
  fftw_complex *in_array, *out_array;
};

#include "system.hpp"

#endif // FFTW_UTILS_HPP
