// globals.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

// These globals are declared in main.cpp, so don't redeclare them
#ifndef MAIN_HPP

#include <complex>

extern int DIM, RANK;
extern std::complex<double> I;


#endif // MAIN_HPP

#endif // GLOBALS_HPP
