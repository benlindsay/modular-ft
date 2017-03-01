// series.cpp
//
// Copyright (c) 2017 Ben Lindsay <benjlindsay@gmail.com>

#include "series.hpp"

void Single_System::init(input::Input_Reader *ir) {
  system = new System(ir);
}
