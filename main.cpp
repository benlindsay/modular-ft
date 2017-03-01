// main.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "main.hpp"

int RANK;
int DIM;
std::complex<double> I(0.0, 1.0);

int main(int argc, const char *argv[])
{
  std::string input_file_name;
  if (argc < 2) {
    // If input file wasn't passed as command line argument,
    // print usage string and exit program
    std::cout << "Usage: " << std::string(argv[0]) <<
      " input_file_name" << std::endl;
    utils::die();
  } else {
    input_file_name = argv[1];
    std::cout << "Input file = " << input_file_name << std::endl;
  }

  input::Input_Reader input_reader;
  // Read input file and check validity of inputs.
  // Information from input file is stored in input_reader object
  input_reader.read_input(input_file_name);
  input_reader.check_inputs();

  Series *series;
  if (input_reader.series_struct.was_read == false ||
      input_reader.series_struct.sub_flag == "single") {
    // Initialize a single simulation unless otherwise specified in input file
    // This sets up all parameters based on the contents of input_reader
    series = new Single_System(&input_reader);
  } else {
    // If otherwise specified in input file,
    // exit program (until this funcitonality is added)
    utils::die("Only single simulations currently supported" +
               input_reader.series_struct.sub_flag);
  }

  if (RANK == 0 ) {
    std::cout << "Lx:" << std::endl;
    for (int i=0; i<DIM; i++) {
      std::cout << series->system->Lx[i] << std::endl;
    }
    std::cout << "Nx:" << std::endl;
    for (int i=0; i<DIM; i++) {
      std::cout << series->system->Nx[i] << std::endl;
    }
  }

  // Run the simulation or series of simulations
  series->run();

  input::flag_struct_t* comp = input_reader.component_struct.components_list[0];
  if (comp->flag == "homo") {
    input::homopoly_struct_t* casted_comp =
      dynamic_cast<input::homopoly_struct_t*>(comp);
    std::cout << "yep" << std::endl;
    std::cout << casted_comp->species_struct.values[0] << std::endl;
    std::cout << casted_comp->n_struct.values[0] << std::endl;
    std::cout << casted_comp->phi_struct.values[0] << std::endl;
  }
  return 0;
}
