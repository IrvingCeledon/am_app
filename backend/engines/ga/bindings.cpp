#include <pybind11/pybind11.h>
#include <pybind11/stl.h>         // Converts std::vector <-> Python lists
#include <pybind11/functional.h>  // Converts std::function <-> Python callables
#include "ga_engine.hpp"

namespace py = pybind11;

PYBIND11_MODULE(ga_module, m) {
    m.doc() = "Binary Genetic Algorithm (GA) Engine";

    // 1. Expose Core Mathematical Types
    py::module_::import("frontend.native.core_module");

    // 2. Expose Engine Configuration
    // Note: EvoConfigBase and GAConfig are flattened into a single class for Python
    py::class_<GAConfig>(m, "GAConfig")
        .def(py::init<>())
        .def_readwrite("generations", &GAConfig::generations)
        .def_readwrite("populationSize", &GAConfig::populationSize)
        .def_readwrite("domains", &GAConfig::domains)
        .def_readwrite("fitness", &GAConfig::fitness)
        .def_readwrite("bits", &GAConfig::bits)
        .def_readwrite("selectionRate", &GAConfig::selectionRate)
        .def_readwrite("crossoverRate", &GAConfig::crossoverRate)
        .def_readwrite("mutationRate", &GAConfig::mutationRate)
        
        // Stopping criteria
        .def_readwrite("use_target", &GAConfig::use_target)
        .def_readwrite("target_fitness", &GAConfig::target_fitness)
        .def_readwrite("use_stagnation", &GAConfig::use_stagnation)
        .def_readwrite("stagnation_patience", &GAConfig::stagnation_patience)
        
        .def_readwrite("minimize", &GAConfig::minimize)
        .def_readwrite("verbosity", &GAConfig::verbosity);

    // 3. Expose the Engine
    py::class_<GAEngine>(m, "GAEngine")
        .def(py::init<const GAConfig&>())
        .def("run", &GAEngine::run);
}
