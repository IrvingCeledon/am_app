#include <pybind11/pybind11.h>
#include <pybind11/stl.h>   // To expose std::vector automatically
#include <pybind11/functional.h>
#include "cga_types.hpp"
#include "cga_config.hpp"
#include "cga.hpp"

namespace py = pybind11;

PYBIND11_MODULE(cga_module, m) {
    m.doc() = "Continuos Genetic Algorithm module";

    // ===== CGA Configuration =====
    py::class_<CGAConfig>(m, "CGAConfig")
        .def(py::init<>())
        .def_readwrite("generations", &CGAConfig::generations)
        .def_readwrite("populationSize", &CGAConfig::populationSize)
        .def_readwrite("domains", &CGAConfig::domains)
        .def_readwrite("fitness", &CGAConfig::fitness)
        .def_readwrite("rates", &CGAConfig::rates)
        .def_readwrite("mutation_scale", &CGAConfig::mutation_scale)
        .def_readwrite("stopping_criteria", &CGAConfig::stopping_criteria);

    // ===== CGA =====
    py::class_<CGA>(m, "CGA")
        .def(py::init<>())
        .def(py::init<const CGAConfig&>(), py::keep_alive<0, 1>())
        .def("run", &CGA::run);
        
    m.def("ackley", &ackley_function);
    m.def("sphere", &sphere_function);
}
