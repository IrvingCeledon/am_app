#include <pybind11/pybind11.h>
#include <pybind11/stl.h>         // Converts std::vector <-> Python lists
#include "types.hpp"

namespace py = pybind11;

PYBIND11_MODULE(core_module, m) {
    m.doc() = "Core Evolutionary Types";

    // 1. Expose enum for "log"
    py::enum_<LogLevel>(m, "LogLevel")
        .value("DEBUG", LogLevel::DEBUG_LVL)
        .value("INFO", LogLevel::INFO_LVL)
        .value("WARNING", LogLevel::WARNING_LVL)
        .value("ERROR", LogLevel::ERROR_LVL)
        .export_values();
        
    py::class_<LogEntry>(m, "LogEntry")
        .def_readonly("level", &LogEntry::level)
        .def_readonly("message", &LogEntry::message);

    // 2. Expose Core Mathematical Types
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
        .def_readonly("finalPopulation", &RunResult::finalPopulation)
        .def_readonly("logs", &RunResult::logs);
}
