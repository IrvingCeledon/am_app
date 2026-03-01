#include <pybind11/pybind11.h>
#include <pybind11/stl.h>         // Converts std::vector <-> Python lists
#include "types.hpp"

namespace py = pybind11;

PYBIND11_MODULE(core_module, m) {
    m.doc() = "Core Evolutionary Types";

    // 1. Expose Core Mathematical Types
    py::class_<Range>(m, "Range")
        .def(py::init<>())
        .def(py::init<double, double>())
        .def_readwrite("min", &Range::min)
        .def_readwrite("max", &Range::max);

    py::class_<Domains>(m, "Domains")
        .def(py::init<>())
        .def_readwrite("ranges", &Domains::ranges)
        .def("dimension", &Domains::dimension);

    py::class_<RunResult>(m, "RunResult")
        .def_readonly("bestFitnesses", &RunResult::bestFitnesses)
        .def_readonly("initialPopulation", &RunResult::initialPopulation)
        .def_readonly("midPopulation", &RunResult::midPopulation)
        .def_readonly("finalPopulation", &RunResult::finalPopulation);
}
