#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/functional.h>
#include "pso_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(pso_module, m) {
    m.doc() = "Motor de Optimizacion por Enjambre de Particulas (PSO)";

    py::module_::import("frontend.native.core_module");

    py::class_<PSOConfig>(m, "PSOConfig")
    .def(py::init<>())
    .def_readwrite("generations", &PSOConfig::generations)
    .def_readwrite("populationSize", &PSOConfig::populationSize)
    .def_readwrite("domains", &PSOConfig::domains)
    .def_readwrite("fitness", &PSOConfig::fitness)
    .def_readwrite("use_target", &PSOConfig::use_target)
    .def_readwrite("target_fitness", &PSOConfig::target_fitness)
    .def_readwrite("use_stagnation", &PSOConfig::use_stagnation)
    .def_readwrite("stagnation_patience", &PSOConfig::stagnation_patience)
    .def_readwrite("verbosity", &PSOConfig::verbosity)
    .def_readwrite("minimize", &PSOConfig::minimize)
    .def_readwrite("w", &PSOConfig::w)
    .def_readwrite("c1", &PSOConfig::c1)
    .def_readwrite("c2", &PSOConfig::c2);

    py::class_<PSOEngine>(m, "PSOEngine")
    .def(py::init<const PSOConfig&>())
    .def("run", &PSOEngine::run);
}
