[![ci](https://github.com/AngryMaciek/angry-computations/workflows/ci/badge.svg?branch=master)](https://github.com/AngryMaciek/angry-computations/actions?query=workflow%3Aci)

# Angry Computations

For specific instructions on how to execute test programs please see the [ci workflow file]().

### numDeriv.c
This directory contains a reimplementation of the well-known [numDeriv package](https://cran.r-project.org/web/packages/numDeriv/index.html) (dedicated to scientific computing) from R into C static library that can be later included in C or C++ code. Although the implementation is based on numDeriv version: 2016.8-1.1 the functionality of this library is limited:  
* Precise calculations are done using Richardson's extrapolation only. Parameters for Richardson's method are explicit arguments for the library functions but the accuracy of the approximations depends on the objective function and may vary for a given set of parameters values. It is strongly advised to always simulate a dataset, adjust the parameters and rebuild the library in order to minimize the error for every model being designed.  
* Side derrivatives are not implemented.  
* Objective functions can have a vector argument but scalar value is a requirement ($f:R^N -> R$). As a result, jacobian function is not implemented.  
