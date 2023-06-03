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
#include <pybind11/pytypes.h>
#include <Eigen/LU>
#include <carma>
#include <armadillo>

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

// https://carma.readthedocs.io/en/latest/introduction.html#ols-example
py::tuple lm(arma::mat& X, arma::colvec& y) {
    // We borrow the data underlying the numpy arrays
    int n = X.n_rows, k = X.n_cols;

    arma::colvec coeffs = arma::solve(X, y);
    arma::colvec resid = y - X * coeffs;

    double sig2 = arma::as_scalar(arma::trans(resid) * resid / (n-k));
    //arma::colvec std_errs = arma::sqrt(sig2 * arma::diagvec( arma::inv(arma::trans(X)*X)) );
    arma::colvec std_errs = arma::diagvec(arma::inv(arma::trans(X)*X));

    // We take ownership of the memory from the armadillo objects and
    // return to python as a tuple containing two Numpy arrays.
    return py::make_tuple(
        carma::col_to_arr(coeffs), // use mat_to_arr() to return arma::mat
        carma::col_to_arr(std_errs)
    );
}
// adapted from https://gallery.rcpp.org/articles/fast-linear-model-with-armadillo/

// binding C++ code to Python
PYBIND11_MODULE(functions, m) {
    m.doc() = "Module: example C++ functions";
    m.def("add2numbers", &add2numbers, py::arg("x"), py::arg("y"));
    m.def("square_a_number", py::vectorize(square_a_number));
    m.def("inverse", &inverse);
    m.def("lm", &lm);
}
