/*=========================================================================

  Program:   C3D: Command-line companion tool to ITK-SNAP
  Module:    Convert3DMain.cxx
  Language:  C++
  Website:   itksnap.org/c3d
  Copyright (c) 2024 Paul A. Yushkevich

  This file is part of C3D, a command-line companion tool to ITK-SNAP

  C3D is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

=========================================================================*/
#include "pybind11/pybind11.h"
#include "pybind11/stl.h"
#include <sstream>
namespace py=pybind11;

using namespace std;

// These are the external utilities that we provide an interface to
extern int cmrep_vskel_main(int, char **);
extern int mesh_tetra_sample_main(int, char **);
extern int mesh_image_sample_main(int, char **);

template <int (*TFunction)(int, char **)>
class FunctionWrapper {
public:
  static void execute(const std::string &str)
  {
    // Split the string using Python
    pybind11::object py_split = pybind11::module_::import("builtins").attr("str").attr("split");
    pybind11::object py_result = py_split(str);
    auto words = py_result.cast<std::vector<std::string>>();

    // Copy the arguments
    int argc = words.size() + 1;
    char **argv = new char* [argc];
    argv[0] = strdup("null");
    for(int i = 1; i < argc; i++)
      argv[i] = strdup(words[i-1].c_str());

    // Call the function
    int rc = TFunction(argc, argv);

    // Delete the arguments
    for(int i = 0; i < argc; i++)
      delete [] argv[i];
    delete [] argv;

    if(rc != 0)
    {
      std::ostringstream oss;
      oss << "Error " << rc << std::endl;
      throw std::runtime_error(oss.str());
    }
  }
};

using cmrep_vskel_wrapper = FunctionWrapper<cmrep_vskel_main>;
using mesh_tetra_sample_wrapper = FunctionWrapper<mesh_tetra_sample_main>;
using mesh_image_sample_wrapper = FunctionWrapper<mesh_image_sample_main>;

PYBIND11_MODULE(picsl_cmrep, m)
{
  m.def("cmrep_vskel", &cmrep_vskel_wrapper::execute, "Run cmrep_vskel tool", py::arg("cmd"));
  m.def("mesh_tetra_sample", &mesh_tetra_sample_wrapper::execute, "Run mesh_tetra_sample tool", py::arg("cmd"));
  m.def("mesh_image_sample", &mesh_image_sample_wrapper::execute, "Run mesh_image_sample tool", py::arg("cmd"));
};
