#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "abc_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(abc_module, m) {
    m.doc() = "Motor de Algoritmo de Colonia de Abejas Artificiales (ABC)";

    py::module_::import("frontend.native.core_module");

    py::class_<ABCConfig>(m, "ABCConfig")
        .def(py::init<>())
        .def_readwrite("generations", &ABCConfig::generations)
        .def_readwrite("populationSize", &ABCConfig::populationSize)
        .def_readwrite("domains", &ABCConfig::domains)
        .def_readwrite("fitness", &ABCConfig::fitness)
        .def_readwrite("use_target", &ABCConfig::use_target)
        .def_readwrite("target_fitness", &ABCConfig::target_fitness)
        .def_readwrite("use_stagnation", &ABCConfig::use_stagnation)
        .def_readwrite("stagnation_patience", &ABCConfig::stagnation_patience)
        .def_readwrite("verbosity", &ABCConfig::verbosity)
        .def_readwrite("use_gbest", &ABCConfig::use_gbest)
        .def_readwrite("minimize", &ABCConfig::minimize)
        .def_readwrite("limit", &ABCConfig::limit);

    py::class_<ABCEngine>(m, "ABCEngine")
        .def(py::init<const ABCConfig&>())
        .def("run", &ABCEngine::run);
}
