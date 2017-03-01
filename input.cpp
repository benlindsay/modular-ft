// initializer.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "input.hpp"

void input::series_struct_t::read(std::istringstream& iss, int num_vals) {
  if (was_read) utils::die(flag + " read twice!");
  if (iss >> sub_flag) {
    // if "single" is part of the first value after "series"
    if (utils::to_lower(sub_flag).find("single") != std::string::npos) {
      sub_flag = "single";
      return;
    } else {
      utils::die("'" + sub_flag + "' in 'series' line not recognized!");
    }
  } else {
    utils::die("Failed reading first sub_flag after 'series' line");
  }
  was_read = true;
}
void input::component_struct_t::read(std::istringstream& iss, int num_vals) {
  std::string sub_flag;
  if (iss >> sub_flag) {
    // if "single" is part of the first value after "series"
    if (utils::to_lower(sub_flag).find("homo") != std::string::npos) {
      components_list.push_back(new homopoly_struct_t(container));
      components_list.back()->read(iss);
      return;
    } else {
      utils::die("'" + sub_flag + "' in 'component' line not recognized!");
    }
  } else {
    utils::die("Failed reading first sub_flag after 'component' line");
  }
  was_read = true;
}

void input::homopoly_struct_t::read(std::istringstream& iss, int num_vals) {
  std::string sub_flag;
  while (iss >> sub_flag) {
    sub_flag = utils::to_lower(sub_flag);
    if (sub_flag == "species") {
      species_struct.read(iss);
    } else if (sub_flag == "n") {
      n_struct.read(iss);
    } else if (sub_flag == "phi") {
      phi_struct.read(iss);
    } else {
      utils::die("'" + sub_flag + "' in 'component homo' line not recognized!");
    }
  }
  was_read = true;
}

void input::output_freq_struct_t::read(std::istringstream& iss, int num_vals) {
  std::string sub_flag;
  while (iss >> sub_flag) {
    sub_flag = utils::to_lower(sub_flag);
    if (sub_flag == "energy") {
      energy_struct.read(iss);
    } else if (sub_flag == "grid") {
      grid_struct.read(iss);
    } else {
      utils::die("'" + sub_flag + "' in 'output_freq' line not recognized!");
    }
  }
  was_read = true;
}

void input::Input_Reader::read_input(std::string input_file_name) {
  // See http://stackoverflow.com/questions/7868936/read-file-line-by-line
  std::ifstream input_file(input_file_name.c_str());
  std::string line;
  bool first_var = true;
  while (std::getline(input_file, line)) {
    // See http://stackoverflow.com/questions/10392405/removing-everything-after-character-and-also-character
    line = line.substr(0, line.find("#"));
    std::istringstream iss(line);
    std::string flag;
    if (iss >> flag) {
      flag = utils::to_lower(flag);
      std::cout << "Flag: " << flag << std::endl;
      if (first_var && flag != "dim") {
        utils::die("First variable in input file must be dim");
      }
      if (flag == "dim") {
        dim_struct.read(iss, 1);
        DIM = dim_struct.values[0];
      } else if (flag == "lx") {
        lx_struct.read(iss, DIM);
      } else if (flag == "nx") {
        nx_struct.read(iss, DIM);
      } else if (flag == "series") {
        series_struct.read(iss);
      } else if (flag == "component") {
        component_struct.read(iss);
      } else if (flag == "max_iter") {
        max_iter_struct.read(iss);
      } else if (flag == "output_freq") {
        output_freq_struct.read(iss);
      } else {
        std::string die_str = "In " + input_file_name +
          ", flag \'" + flag + "\'not recognized.";
        utils::die(die_str);
      }
      first_var = false;
    }
  }
}

void input::Input_Reader::check_inputs() {
  if (DIM < 1 || DIM > 3)  utils::die("DIM must be 1, 2, or 3");
  if (!lx_struct.was_read) utils::die("Must set Lx in input file!");
  if (!(lx_struct.values.size() == DIM)) utils::die("Length of Lx != DIM");
  if (!nx_struct.was_read) utils::die("Must set Nx in input file!");
  if (!(nx_struct.values.size() == DIM)) utils::die("Length of Nx != DIM");
}

// System* input::Input_Reader::generate_first_system() {
//   System *first_system = new System();
//   first_system->Lx = lx_struct.values;
//   first_system->Nx = nx_struct.values;
//   return first_system;
// }

// Series* input::Input_Reader::generate_series() {
//   // System *first_system = generate_first_system();
//   if (series_struct.was_read == false || series_struct.sub_flag == "single") {
//     return new Single_System(this);
//   } else {
//     utils::die("Only single simulations currently supported" +
//                series_struct.sub_flag);
//   }
// }
