// fftw_utils.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "fftw_utils.hpp"
#include <iostream>

void FFTW_Utils::init(input::Input_Reader *ir) {
  if (RANK == 0) {
    std::cout << "==== Initializing FFTW_Utils ====" << std::endl;
  }
  int *Nx = system->Nx;
  int NxL[DIM];
  int M = system->M;
  int ML;

  // fftw functions need ptrdiff_t type variables so it can do
  // pointer math stuff reliably or something
  ptrdiff_t dim_tmp = DIM;
  // ptrdiff_t Nx_rev[DIM];
  int Nx_rev[DIM];
  // Fill reverse Nx array because we'll be slicing in z-direction
  // instead of the x-direction
  for (int i = 0; i < DIM; i++) {
    Nx_rev[i] = Nx[DIM - i - 1];
  }

#ifdef MPI
  // Number of points in local array in the DIM-1 direction
  // and the index in the DIM-1 direction at which the local
  // array starts
  ptrdiff_t Nz_local, z_local_0;

  // Get the local array length, filling the Nz_local and z_local_0 variables
  // along the way. Those represent the number of points in the DIM-1 direction
  // (locally) and the index in the DIM-1 direction at which the local
  // array starts
  ML = fftw_mpi_local_size_many(dim_tmp, Nx_rev, 1, 0,
                                MPI_COMM_WORLD, &Nz_local, &z_local_0);
  system->ML = ML;

  // Fill the local Nx array (NxL), which is the same as Nx except in the DIM-1
  // direction
  for (int i = 0; i < DIM - 1; i++) {
    NxL[i] = Nx[i];
  }
  NxL[DIM - 1] = Nz_local;

  alloc_wrapper_fftw_complex(&in_array);
  alloc_wrapper_fftw_complex(&out_array);
  fwd_plan = fftw_mpi_plan_dft(DIM, Nx_rev, in_array, out_array,
                               MPI_COMM_WORLD, FFTW_FORWARD, FFTW_MEASURE);
  bck_plan = fftw_mpi_plan_dft(DIM, Nx_rev, in_array, out_array,
                               MPI_COMM_WORLD, FFTW_BACKWARD, FFTW_MEASURE);
#else
  // If not using MPI, the local array length equals the global array length
  ML = M;
  system->ML = ML;
  for (int i = 0; i < DIM; i++) {
    NxL[i] = Nx[i];
  }
  alloc_wrapper_fftw_complex(&in_array);
  alloc_wrapper_fftw_complex(&out_array);
  fwd_plan = fftw_plan_dft(dim_tmp, Nx_rev, in_array, out_array,
                           FFTW_FORWARD, FFTW_MEASURE);
  bck_plan = fftw_plan_dft(dim_tmp, Nx_rev, in_array, out_array,
                           FFTW_FORWARD, FFTW_MEASURE);
#endif // MPI

  // Set the ML variable in the system object
  system->ML = ML;
  for (int i = 0; i < DIM; i++) {
    system->NxL[i] = NxL[i];
  }
}

FFTW_Utils::~FFTW_Utils() {
  fftw_free(in_array);
  fftw_free(out_array);
}

// Allocate 1D complex double array of length ML. If you have an array defined
// like this:
//      std::complex<double> *my_array;
// and a pointer to an FFTW_Utils instance declared as:
//      FFTW_Utils *my_fftw_utils;
// then you would allocate my_array like this:
//      my_fftw_utis->alloc_wrapper(&my_array);
void FFTW_Utils::alloc_wrapper(std::complex<double> **array) {
  int ML = system->ML;
  int mem_size = ML * sizeof(std::complex<double>);
  (*array) = (std::complex<double>*) fftw_malloc( mem_size );
  for (int i = 0; i < ML; i++) {
    (*array)[i] = 0.0;
  }
  memory_alloced += mem_size;
  if (RANK == 0) {
    std::cout << "1D allocation. Memory alloced: " << memory_alloced << std::endl;
  }
}

// This is the same as the alloc_wrapper function, but it takes allocates arrays
// of type fftw_complex
void FFTW_Utils::alloc_wrapper_fftw_complex(fftw_complex **array) {
  int ML = system->ML;
  int mem_size = ML * sizeof(fftw_complex);
  (*array) = (fftw_complex*) fftw_malloc( mem_size );
  for (int i = 0; i < ML; i++) {
    (*array)[i][0] = 0.0;
    (*array)[i][1] = 0.0;
  }
  memory_alloced += mem_size;
  if (RANK == 0) {
    std::cout << "1D allocation. Memory alloced: " << memory_alloced << std::endl;
  }
}

// Forward transform
void FFTW_Utils::fft_fwd_wrapper(std::complex<double> *in,
                                 std::complex<double> *out) {

  int ML = system->ML;
  // Store fft input
  for (int i = 0; i < ML; i++) {
    in_array[i][0] = real(in[i]);
    in_array[i][1] = imag(in[i]);
  }

  fftw_execute(fwd_plan);

  double norm = 1.0 / double(system->M);

  // Pass result to output array
  for (int i = 0; i < ML; i++) {
    out[i] = ( out_array[i][0] + I * in_array[i][1] ) * norm ;
  }

}

// Backward transform
void FFTW_Utils::fft_bck_wrapper(std::complex<double> *in,
                                 std::complex<double> *out) {

  int ML = system->ML;
  // Store fft input
  for (int i = 0; i < ML; i++) {
    in_array[i][0] = real(in[i]);
    in_array[i][1] = imag(in[i]);
  }

  fftw_execute(fwd_plan);

  double norm = 1.0 / double(system->M);

  // Pass result to output array
  for (int i = 0; i < ML; i++) {
    out[i] = ( out_array[i][0] + I * in_array[i][1] ) * norm ;
  }

}
