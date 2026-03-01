#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "functions.hpp"

namespace py = pybind11;

PYBIND11_MODULE(benchmarks_module, m) {
    m.doc() = "Test functions for metaheuristics";

    m.def("cost_function_a", &cost_function_a, "Cubic polynomial function");
    m.def("cost_function_b", &cost_function_b, "Polynomial function of degree 4");
    m.def("cost_function_c", &cost_function_c, "2D complex function");
    m.def("ackley", &ackley_function, "N-Dimensional Ackley Function");
    m.def("sphere", &sphere_function, "N-Dimensional Sphere Function");
    m.def("eggholder", &eggholder_function, "2D Eggholder Function");
    m.def("rosenbrock", &rosenbrock_function, "2D Rosenbrock Function");
    m.def("michalewicz", &michalewicz_function, "Michalewicz Function");
}
