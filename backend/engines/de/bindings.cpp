#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "de_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(de_module, m) {
    m.doc() = "Differential Evolution (DE) Engine";
    py::module_::import("frontend.native.core_module");

    py::class_<DEConfig>(m, "DEConfig")
    .def(py::init<>())
    .def_readwrite("generations", &DEConfig::generations)
    .def_readwrite("populationSize", &DEConfig::populationSize)
    .def_readwrite("domains", &DEConfig::domains)
    .def_readwrite("fitness", &DEConfig::fitness)
    .def_readwrite("use_target", &DEConfig::use_target)
    .def_readwrite("target_fitness", &DEConfig::target_fitness)
    .def_readwrite("use_stagnation", &DEConfig::use_stagnation)
    .def_readwrite("stagnation_patience", &DEConfig::stagnation_patience)
    .def_readwrite("minimize", &DEConfig::minimize)
    .def_readwrite("verbosity", &DEConfig::verbosity)
    .def_readwrite("F", &DEConfig::F)
    .def_readwrite("CR", &DEConfig::CR);

    py::class_<DEEngine>(m, "DEEngine")
    .def(py::init<const DEConfig&>())
    .def("run", &DEEngine::run);
}
