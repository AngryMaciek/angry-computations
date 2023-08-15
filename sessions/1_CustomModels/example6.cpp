#include <iostream>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_roots.h>
#include <gsl/gsl_deriv.h>

// f(x) = x^2 - 4
double function(double x, void* params){
    return x * x - 4.0;
}

int main(){

    // root search

    gsl_function F;
    F.function = &function;
    F.params = nullptr;
    double xLower = 1.0;
    double xUpper = 3.0;

    const gsl_root_fsolver_type* solverType = gsl_root_fsolver_brent;
    gsl_root_fsolver* solver = gsl_root_fsolver_alloc(solverType);
    gsl_root_fsolver_set(solver, &F, xLower, xUpper);

    int status;
    int iter = 0;
    int maxIter = 100;
    double root;
    double epsabs = 0.0;
    double epsrel = 1e-6;

    do{
        iter++;
        status = gsl_root_fsolver_iterate(solver);
        root = gsl_root_fsolver_root(solver);
        xLower = gsl_root_fsolver_x_lower(solver);
        xUpper = gsl_root_fsolver_x_upper(solver);
        status = gsl_root_test_interval(xLower, xUpper, epsabs, epsrel);
        if (status == GSL_SUCCESS){
            std::cout << "Function root found at x = " << root << std::endl;
            break;
        }
    } while (status == GSL_CONTINUE && iter < maxIter);

    gsl_root_fsolver_free(solver);

    std::cout << "=====" << std::endl;

    // derrivative
    double result, abserr;
    double arg = 2.1;
    double h = 1e-8;
    gsl_deriv_central(&F, arg, h, &result, &abserr);
    std::cout << "f'(x) = " << result << " +/- " << abserr << std::endl;

    return 0;
}
