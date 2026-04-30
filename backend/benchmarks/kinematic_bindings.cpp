#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "kinematic.hpp"

namespace py = pybind11;

PYBIND11_MODULE(kinematic_module, m) {
    m.doc() = "Inverse Kinematics Evaluator";

    py::class_<IKEvaluator>(m, "IKEvaluator")
    .def(py::init<const std::vector<double>&,
         const std::vector<double>&,
         const std::vector<double>&,
         double,
         double>(),
         py::arg("target"),
         py::arg("prev_posture") = std::vector<double>{},
         py::arg("target_q") = std::vector<double>{},
         py::arg("jmp") = 0.5,
         py::arg("op") = 10.0)
    .def("__call__", &IKEvaluator::operator());
}
