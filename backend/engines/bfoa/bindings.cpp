#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "bfoa_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(bfoa_module, m) {
    m.doc() = "Bacterial Foraging Optimization Algorithm (BFOA) Engine";

    py::module_::import("frontend.native.core_module");

    py::class_<BFOAConfig>(m, "BFOAConfig")
    .def(py::init<>())
    .def_readwrite("generations", &BFOAConfig::generations)
    .def_readwrite("populationSize", &BFOAConfig::populationSize)
    .def_readwrite("domains", &BFOAConfig::domains)
    .def_readwrite("fitness", &BFOAConfig::fitness)
    .def_readwrite("use_target", &BFOAConfig::use_target)
    .def_readwrite("target_fitness", &BFOAConfig::target_fitness)
    .def_readwrite("use_stagnation", &BFOAConfig::use_stagnation)
    .def_readwrite("stagnation_patience", &BFOAConfig::stagnation_patience)
    .def_readwrite("minimize", &BFOAConfig::minimize)
    .def_readwrite("verbosity", &BFOAConfig::verbosity)

    // BFOA specific fields
    .def_readwrite("chemotactic_steps", &BFOAConfig::chemotactic_steps)
    .def_readwrite("reproduction_steps", &BFOAConfig::reproduction_steps)
    .def_readwrite("elimination_dispersal_steps", &BFOAConfig::elimination_dispersal_steps)
    .def_readwrite("run_length", &BFOAConfig::run_length)
    .def_readwrite("step_size", &BFOAConfig::step_size)
    .def_readwrite("elimination_rate", &BFOAConfig::elimination_rate)

    // Swarming parameters
    .def_readwrite("d_attract", &BFOAConfig::d_attract)
    .def_readwrite("w_attract", &BFOAConfig::w_attract)
    .def_readwrite("h_repel", &BFOAConfig::h_repel)
    .def_readwrite("w_repel", &BFOAConfig::w_repel);

    py::class_<BFOAEngine>(m, "BFOAEngine")
    .def(py::init<const BFOAConfig&>())
    .def("run", &BFOAEngine::run);
}
