#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "cga_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(cga_module, m) {
    m.doc() = "Motor de Algoritmo Genético Continuo (CGA)";

    // 1. Core types 
    py::module_::import("frontend.native.core_module");

    // 2. CGA-specific configuration
    py::class_<CGAConfig>(m, "CGAConfig")
        .def(py::init<>())
        .def_readwrite("generations", &CGAConfig::generations)
        .def_readwrite("populationSize", &CGAConfig::populationSize)
        .def_readwrite("domains", &CGAConfig::domains)
        .def_readwrite("fitness", &CGAConfig::fitness)
        .def_readwrite("selectionRate", &CGAConfig::selectionRate)
        .def_readwrite("crossoverRate", &CGAConfig::crossoverRate)
        .def_readwrite("mutationRate", &CGAConfig::mutationRate)
        .def_readwrite("mutationScale", &CGAConfig::mutationScale)
        
        // Stopping criteria
        .def_readwrite("use_target", &CGAConfig::use_target)
        .def_readwrite("target_fitness", &CGAConfig::target_fitness)
        .def_readwrite("use_stagnation", &CGAConfig::use_stagnation)
        .def_readwrite("stagnation_patience", &CGAConfig::stagnation_patience);

    // 3. The Engine
    py::class_<CGAEngine>(m, "CGAEngine")
        .def(py::init<const CGAConfig&>())
        .def("run", &CGAEngine::run);
}
