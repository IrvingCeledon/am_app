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

    m.def("get_forward_kinematics", &get_forward_kinematics, "Calculates Homogeneous Transformation Matrix");
    m.def("get_quaternion", &get_quaternion, "Extracts the 3x3 rotation matrix from a 5x5 Homogeneous matrix");
}
