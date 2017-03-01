// inputializer.hpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#ifndef INPUT_HPP
#define INPUT_HPP

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "globals.hpp"
#include "utils.hpp"
#include "component.hpp"

namespace input {

  class Input_Reader;

  struct flag_struct_t {
    flag_struct_t(input::Input_Reader *_container, std::string _flag) :
      container(_container), flag(_flag), was_read(false) { };
    virtual void read(std::istringstream& iss, int num_vals) = 0;
    virtual void read(std::istringstream& iss) = 0;
    input::Input_Reader *container;
    std::string flag;
    bool was_read;
  };

  template <typename T>
  struct vector_struct_t : public flag_struct_t {
    vector_struct_t(input::Input_Reader *_container, std::string _flag) :
      flag_struct_t(_container, _flag) { };
    void read(std::istringstream& iss, int num_vals) {
      if (was_read) utils::die(flag + " read twice.");
      for (int i = 0; i < num_vals; i++) {
        T tmp_val;
        if (!(iss >> tmp_val)) utils::die("Reading '" + flag + "' failed.");
        values.push_back(tmp_val);
      }
      was_read = true;
    };
    void read(std::istringstream& iss) { read(iss, 1); };
    std::vector<T> values;
  };

  struct series_struct_t : public flag_struct_t {
    series_struct_t(input::Input_Reader *_container) :
      flag_struct_t(_container, "series") { };
    void read(std::istringstream& iss, int num_vals);
    void read(std::istringstream& iss) { read(iss, 1); };
    std::string sub_flag;
  };

  struct homopoly_struct_t : public flag_struct_t {
    homopoly_struct_t(input::Input_Reader *_container) :
      flag_struct_t(_container, "homo"),
      species_struct(_container, "species"),
      n_struct(_container, "n"),
      phi_struct(_container, "phi")
    { };
    void read(std::istringstream& iss, int num_vals);
    void read(std::istringstream& iss) { read(iss, 1); };
    input::vector_struct_t<std::string> species_struct;
    input::vector_struct_t<int> n_struct;
    input::vector_struct_t<double> phi_struct;
  };

  struct component_struct_t : public flag_struct_t {
    component_struct_t(input::Input_Reader *_container) :
      flag_struct_t(_container, "component") { };
    void read(std::istringstream& iss, int num_vals);
    void read(std::istringstream& iss) { read(iss, 1); };
    std::vector<flag_struct_t*> components_list;
  };

  struct output_freq_struct_t : public flag_struct_t {
    output_freq_struct_t(input::Input_Reader *_container) :
      flag_struct_t(_container, "output_freq"),
      energy_struct(_container, "energy"),
      grid_struct(_container, "grid")
    { };
    void read(std::istringstream& iss, int num_vals);
    void read(std::istringstream& iss) { read(iss, 1); };
    input::vector_struct_t<int> energy_struct;
    input::vector_struct_t<int> grid_struct;
  };

  class Input_Reader {
   public:
    Input_Reader() :
      dim_struct(this, "dim"),
      lx_struct(this, "lx"),
      nx_struct(this, "nx"),
      series_struct(this),
      component_struct(this),
      max_iter_struct(this, "max_iter"),
      output_freq_struct(this)
    { };
    input::vector_struct_t<int> dim_struct;
    input::vector_struct_t<double> lx_struct;
    input::vector_struct_t<int> nx_struct;
    input::series_struct_t series_struct;
    input::component_struct_t component_struct;
    input::vector_struct_t<int> max_iter_struct;
    input::output_freq_struct_t output_freq_struct;

    void read_input(std::string input_file_name);
    void parse_line(std::string line);
    void check_inputs();
    // System* generate_first_system();
    // Series* generate_series();
  };

}

#endif // INPUT_HPP
