/*
###############################################################################
#
#   Test: interfacing from Python to C++
#   Based on:
#   https://github.com/pybind/pybind11
#
#   AUTHOR: Maciej_Bak
#   AFFILIATION: Swiss_Institute_of_Bioinformatics
#   CONTACT: very.angry.maciek@gmail.com
#   CREATED: 05-10-2020
#   LICENSE: MIT
#
###############################################################################
*/

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/eigen.h>
#include <Eigen/LU>
#include <armadillo>
#include <carma>
namespace py = pybind11;

double add2numbers(double i, double j) {
    return i + j;
}

double square_a_number(double x) {
    return x * x;
}

Eigen::MatrixXd inverse(Eigen::MatrixXd m) {
    return m.inverse();
}

// binding C++ code to Python
PYBIND11_MODULE(functions, m) {
    m.doc() = "Module: example C++ functions";
    m.def("add2numbers", &add2numbers, py::arg("x"), py::arg("y"));
    m.def("square_a_number", py::vectorize(square_a_number));
    m.def("inverse", &inverse);
}
