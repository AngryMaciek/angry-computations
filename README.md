[![Open in GitHub Codespaces](https://badgen.net/badge/icon/Open%20in%20GitHub%20Codespaces?icon=github&label&color=black)](https://github.com/codespaces/new?hide_repo_select=true&ref=master&repo=648110302&skip_quickstart=true&machine=basicLinux32gb&geo=EuropeWest&devcontainer_path=.devcontainer%2Fdevcontainer.json)
[![ci](https://github.com/AngryMaciek/efcomp/workflows/ci/badge.svg?branch=master)](https://github.com/AngryMaciek/efcomp/actions?query=workflow%3Aci)
[![GitHub license](https://img.shields.io/github/license/AngryMaciek/efcomp)](https://github.com/AngryMaciek/efcomp/blob/master/LICENSE)

# Angry Computations 😡

For specific instructions on how to execute test programs please see the [ci workflow file](https://github.com/AngryMaciek/efcomp/blob/master/.github/workflows/ci.yml).

### numDeriv.c
This directory contains a reimplementation of the well-known [numDeriv package](https://cran.r-project.org/web/packages/numDeriv/index.html) (dedicated to scientific computing) from R into C static library that can be later included in C or C++ code. Although the implementation is based on numDeriv version: 2016.8-1.1 the functionality of this library is limited:  
* Precise calculations are done using Richardson's extrapolation; parameters for the method are explicit arguments for the library functions but the accuracy of the approximations depends on the objective function and may vary for a given set of parameters values.
* Side derrivatives are not implemented.  
* Objective functions can have a vector argument but scalar value is a requirement ($f:R^N -> R$).

### angry-statistical-computing
Toy exampes of how to use C++ [Armadillo](http://arma.sourceforge.net/) library for linear algebra and simple statistics as well as other custom functions useful for statistical modelling. These resources should be enough to implement complex Bayesian models, fit the parameters efficiently (likelihood optimization) and minimize the runtime on big datasets. Functions' logic and sygnatures are described within the main source file of this directory. Most notably, we shall use three additional C files with external procedures:
* `numDeriv.c`: as above, functions to approximate gradient and hessian of a given multivariate function
* `zeroin.c`: root finding for an univariate function
* `modoptim.c`: a modified version of the C file with a method to minimize a given multivariate function

### ToyRcpp
This is a very small directory that presents the libraries needed for efficient statistical computing in [R interfacing to C++](http://adv-r.had.co.nz/Rcpp.html). Functions presented are useful to build statistical models based on likelihood and optimize their parameters. These techniques should be enough to implement complex Bayesian models, fit the parameters efficiently and optimize the runtime on big datasets. Functions' logic and sygnatures are described within the source code. Additionally, examples for [RcppArmadillo](https://github.com/RcppCore/RcppArmadillo) and [RcppEigen](https://github.com/RcppCore/RcppEigen) are provided.

### ToySnake
Similarily as above, this directory contains toy examples of how to utilize [pybind11](https://pybind11.readthedocs.io/en/stable) and [CARMA](https://github.com/RUrlus/carma) to interface between Python/NumPy and C++ with _Eigen_ and _Armadillo_ libraries for efficient computing.
