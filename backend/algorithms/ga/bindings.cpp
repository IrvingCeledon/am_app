#include <pybind11/pybind11.h>
#include <pybind11/stl.h>   // To expose std::vector automatically
#include <pybind11/functional.h>
#include "ga_types.hpp"
#include "ga_config.hpp"
#include "chromosome.hpp"
#include "individual.hpp"
#include "population.hpp"
#include "ga.hpp"

namespace py = pybind11;

PYBIND11_MODULE(ga_module, m) {
    m.doc() = "Genetic Algorithm module";

    // ===== Types =====
    py::class_<Range>(m, "Range")
        .def_readwrite("min", &Range::min)
        .def_readwrite("max", &Range::max);

    py::class_<Domains>(m, "Domains")
        .def_readwrite("x", &Domains::x)
        .def_readwrite("y", &Domains::y);

    py::class_<Rates>(m, "Rates")
        .def_readwrite("selection", &Rates::selection)
        .def_readwrite("crossover", &Rates::crossover)
        .def_readwrite("mutation", &Rates::mutation);
        
    py::class_<RunResult>(m, "RunResult")
        .def_readwrite("bestFitnesses", &RunResult::bestFitnesses)
        .def_readwrite("initialPopulation", &RunResult::initialPopulation)
        .def_readwrite("midPopulation", &RunResult::midPopulation)
        .def_readwrite("finalPopulation", &RunResult::finalPopulation);

    py::class_<GAConfig>(m, "GAConfig")
        .def(py::init<>())
        .def_readwrite("generations", &GAConfig::generations)
        .def_readwrite("populationSize", &GAConfig::populationSize)
        .def_readwrite("bits", &GAConfig::bits)
        .def_readwrite("domains", &GAConfig::domains)
        .def_readwrite("fitness", &GAConfig::fitness)
        .def_readwrite("rates", &GAConfig::rates);

    // ===== Chromosome =====
    py::class_<Chromosome>(m, "Chromosome")
        .def(py::init<>())
        .def(py::init<uint16_t, uint16_t>());

    // ===== Individual =====
    py::class_<Individual>(m, "Individual")
        .def(py::init<>())
        .def(py::init<const Chromosome&, double, double, double>());

    // ===== Population =====
    py::class_<Population>(m, "Population")
        .def(py::init<>())
        .def(py::init<size_t>());

    // ===== GA =====
    py::class_<GA>(m, "GA")
        .def(py::init<>())
        .def(py::init<const GAConfig&>(), py::keep_alive<0, 1>())
        .def("run", &GA::run);
        
    m.def("f1", &cost_function_a);
    m.def("f2", &cost_function_b);
    m.def("f3", &cost_function_c);
}
