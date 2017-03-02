// array_utils.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "array_utils.hpp"
#include <iostream>

// Takes integer "local_index" in [0, ML) and returns the the correct global
// index in [0, M)
// Known in other code as "unstack_stack"
int Array_Utils::get_global_index(int local_index) {
#ifdef MPI
  // Get the number of points in all dimensions at each z-value
  int n_points_per_z = 1;
  for (int d = 0; d < DIM - 1; d++) {
    n_points_per_z *= system->Nx[d];
  }
  // Add the number of points in all processors of lower rank to the
  // local_index to get the global index
  return local_index + system->z_local_0 * n_points_per_z;
#else
  // If there's no parallelization, global_index = local_index
  return local_index;
#endif
}

// Takes "n_global" array of indices corresponding to one global point and
// returns global index corresponding to that point
// Known in other code as "stack_input"
int Array_Utils::get_global_index(int *n_global) {
  int global_index = n_global[DIM - 1];
  for (int d = DIM - 2; d >= 0; d--) {
    global_index = n_global[d] + system->Nx[d] * global_index;
  }
  return global_index;
}

// Takes "n_local" array of indices corresponding to one local point and
// returns local index corresponding to that point
// Known in other code as "stack_local"
int Array_Utils::get_local_index(int *n_local) {
  int local_index = n_local[DIM - 1];
  for (int d = DIM - 2; d >= 0; d--) {
    local_index = n_local[d] + system->NxL[d] * local_index;
  }
  return local_index;
}

// Receives "local_index" in [0, ML) and fills array "n_local" with array of
// indices corresponding to "local_index"
// Known in other code as "unstack_local"
void Array_Utils::fill_local_index_array(int local_index, int *n_local) {
  int *NxL = system->NxL;
  if (DIM==3) {
    n_local[2] = local_index/NxL[1]/NxL[0];
    n_local[1] = local_index/NxL[0] - n_local[2]*NxL[1];
    n_local[0] = local_index - (n_local[1] + n_local[2]*NxL[1])*NxL[0];
    return;
  }
  else if (DIM==2) {
    n_local[1] = local_index/NxL[0];
    n_local[0] = (local_index - n_local[1]*NxL[0]);
    return;
  }
  else { // DIM == 1
    n_local[0] = local_index;
    return;
  }
}

// Receives "local_index" in [0, ML) and fills array "n_global" with array of
// indices corresponding to "global_index"
// Known in other code as "unstack", but that took a global_index instead of a
// local_index
void Array_Utils::fill_global_index_array(int local_index, int *n_global) {
  // First, fill n_global with the local index array.
  // We're done if not using MPI, since local = global
  fill_local_index_array(local_index, n_global);
#ifdef MPI
  // If using MPI, add z_local_0 to the last dimension so it represents
  // the global value
  n_global[DIM - 1] += system->z_local_0;
#endif
}

// Given "local_index" in [0, ML), return the square distance from that point
// to the origin and fill the "r" array with distance vector from origin, where
// each element is in (-Lx[d]/2, Lx[d]/2], where Lx[d] is the length of the box
// in dimension d.
// Known in other code as "get_r"
double Array_Utils::get_r2_and_fill_r(int local_index, double *r) {
  double r2 = 0.0;
  int global_index, n_global[DIM];
  double *dx = system->dx;
  double *Lx = system->Lx;
  fill_global_index_array(local_index, n_global);
  for (int d = 0; d < DIM; d++) {
    r[d] = dx[d] * double(n_global[d]);
    if (r[d] > Lx[d]/2.0) {
      r[d] -= Lx[d];
    }
    r2 += r[d]*r[d];
  }
  return r2;
}

// Receives "local_index" in [0, ML), fills the "k" array and returns
// the magnitude squared of the "k" array
double Array_Utils::get_k2_and_fill_k(int local_index, double *k) {
  int *Nx = system->Nx;
  double *Lx = system->Lx;
  double kmag = 0.0;
  int n_global[DIM];
  fill_global_index_array(local_index, n_global);
  for (int d = 0; d < DIM; d++) {
    if ( double(n_global[d]) < double(Nx[d]) / 2.) {
      k[d] = 2*PI*double(n_global[d])/Lx[d];
    }
    else {
      k[d] = 2*PI*double(n_global[d]-Nx[d])/Lx[d];
    }
  }
  // Check if this point has a nyquist mode
  bool filter_nyquist = false;
  for (int d = 0; d < DIM; d++) {
    if (Nx[d] % 2 == 0 && n_global[d] == Nx[d] / 2) {
      filter_nyquist = true;
    }
  }
  // If this point has a nyquist mode, zero the k array
  if (filter_nyquist) {
    for (int d = 0; d < DIM; d++) {
      k[d] = 0.0;
    }
    return 0.0;
  }
  for (int d = 0; d < DIM; d++) {
    kmag += k[d]*k[d];
  }
  return kmag;
}

// Sets the average of array to zero
void Array_Utils::zero_average(std::complex<double> *array) {
  std::complex<double> integ;
  integ = integ_trapPBC(array);
  integ *= (1.0 / system->V);
  for (int i = 0; i < system->ML; i++) {
    array[i] -= integ;
  }
}

// Return integral of array
std::complex<double> Array_Utils::integ_trapPBC(std::complex<double> *array) {
  std::complex<double> sum = std::complex<double>(0.0,0.0);
  for (int i = 0; i < system->ML; i++) {
    sum += array[i];
  }
  for (int d = 0; d < DIM; d++) {
    sum *= system->dx[d];
  }
#ifdef MPI
  // Accumulate sums from all other processors if using MPI
  double real_local = real(sum);
  double imag_local = imag(sum);
  double real_global, imag_global;
  MPI_Allreduce(&real_local, &real_global, 1,
                MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  MPI_Allreduce(&imag_local, &imag_global, 1,
                MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  sum = real_global + I * imag_global;
#endif
  return sum;
}
