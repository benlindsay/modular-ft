// component.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <vector>
#include <complex>
#include "globals.hpp"
#include "fftw_utils.hpp"

class FFTW_Utils;

namespace component {

  enum Species_Type { NEUTRAL, A_TYPE, B_TYPE, C_TYPE, D_TYPE };

  inline Species_Type to_species_type(std::string str) {
    if (str == "N") return NEUTRAL;
    else if (str == "A") return A_TYPE;
    else if (str == "B") return B_TYPE;
    else if (str == "C") return C_TYPE;
    else if (str == "D") return D_TYPE;
    else {
      utils::die("'" + str + "' not recognized " +
                 "in to_species_type function!");
    }
  }

  // Species_Type foo_bar() { return A_TYPE; }

  struct Species {
    Species(Species_Type type) :
      species_type(type), rho(NULL), rho_avg(NULL), rho_hat(NULL) { };
    ~Species();
    void allocate_arrays(FFTW_Utils *fftw_utils);

    Species_Type species_type;
    std::complex<double> *rho;
    std::complex<double> *rho_avg;
    std::complex<double> *rho_hat;
  };

  class Component {
   public:
    virtual void calc_density(FFTW_Utils *fftw_utils) = 0;
    std::vector<Species*> species_list;
    double n;
    double phi;
  };

  class Homopolymer : public Component {
   public:
    Homopolymer(Species_Type species_type, int N) : N(N) {
      std::cout << "adding homopoly species " << species_type << std::endl;
      species_list.push_back(new Species(species_type));
    };
    void calc_density(FFTW_Utils *fftw_utils) {
      std::cout << "calculating homopolymer density" << std::endl;
    };
    int N;
  };
}

#endif // COMPONENT_HPP
