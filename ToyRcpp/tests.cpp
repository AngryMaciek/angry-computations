// Maciej Bak
// Swiss Institute of Bioinformatics
// 4.06.2019

#include <Rcpp.h>
#include "modified_optim.c"
extern "C" {
#include "zeroin.c"
}
using namespace Rcpp;


// ################## ROOT APROXIMATION FOR AN UNIVARIATE FUNCTION ##################

// A data strucutre to hold the input data required
// to calculate values of the objective function
class uniroot_BOX {
  public:
    double x,y;
    uniroot_BOX(double x_, double y_) :
      x(x_), y(y_) {}
};


// Objective function for the root finding procedure
// All the logic related to data access should be here
// The sygnature is fixed!
// Arg1: double x, the argument of the objective function
// Arg2: void*, a void pointer to the data structure
double uniroot_objF(double x, void* void_pointer_box){
  // de-reference the void pointer to a pointer to uniroot_BOX instance
  uniroot_BOX *pointer_box = static_cast<uniroot_BOX*>(void_pointer_box); 
  // calculate the value of the objective function at a given argument
  // F(X) = 1/X + exp(X+1/x) - y
  double value = 1/x + exp(x+1/(*pointer_box).x) - (*pointer_box).y;
  return value;
}


// [[Rcpp::export]]
NumericVector calculate_roots(NumericVector x, NumericVector y) {

  const int len = x.size();
  NumericVector roots(len);
  double fx_upper, fx_lower;
  int max_iter;
  double eps; // epsilon for the root approximation
  // define the endpoints for the search
  double X_upper = 10.0;
  double X_lower = 1;
  // put the data into the uniroot_BOX object
  uniroot_BOX box(0,0);
  void* void_pointer_box = &box;

  // find roots for every (x_i,y_i) separately:
  for (int i = 0; i < len; i++) {

    // assign parameters to the box
    box.x = x[i];
    box.y = y[i];

    // calculate the value of the objective function at the endpoints
    fx_upper = uniroot_objF(X_upper,void_pointer_box);
    fx_lower = uniroot_objF(X_lower,void_pointer_box);
    // stop if the signs at the endpoints are the same
    if (fx_upper * fx_lower >= 0 ) Rcpp::stop("Incorrect range for bisection");

    // R_zeroin2 procedure:
    // max_iter and eps get re-assigned!
    // they need to be reset after every iteration
    // The sygnature: endpoints, values at endpoints, pointer to the objective function,
    // void pointer to the data, references to max_iter and eps variables
    max_iter = 10000;
    eps = pow(10,-20);
    roots[i] = R_zeroin2(X_lower, X_upper, fx_lower, fx_upper, *uniroot_objF, void_pointer_box, &eps, &max_iter);
  }
  return roots;
}

// ##################################################################################


// #################### MINIMISATION OF A MULTIVARIATE FUNCTION #####################

// A data strucutre to hold the input data and parameters required
// to calculate value of the objective function and its gradient
class optim_BOX {
  public:
    NumericMatrix xy;
    int index;
    optim_BOX(NumericMatrix xy_, int index_) :
      xy(xy_), index(index_) {}
};


// Objective function for the minimisation procedure
// All the logic related to data access should be here
// The sygnature is fixed!
// Arg1: int n, the number of arguments of the objective function
// Arg2: double*, a pointer to the vector with arguments to the objective function
// Arg3: void*, a void pointer to the data structure
double optim_objF(int n, double* args, void* void_pointer_box) {
  // de-reference the void pointer to a pointer to optim_BOX instance
  optim_BOX *pointer_box = static_cast<optim_BOX*>(void_pointer_box);
  int index = (*pointer_box).index;
  NumericMatrix xy = (*pointer_box).xy;
  double x = xy(index,0);
  double y = xy(index,1);
  // calculate the value of the objective function at a given argument
  // F(X1,X2) = x^X1 + 2^(-X1) + y^X2 + 2^(-X2)
  double value = pow(x,args[0]) + pow(2,-args[0]) + pow(y,args[1]) + pow(2,-args[1]);
  return value;
}


// Gradient of the objective function for the minimisation procedure
// All the logic related to data access should be here
// The sygnature is fixed!
// Arg1: int n, the number of arguments of the objective function
// Arg2: double*, a pointer to the vector with arguments to the objective function
// Arg3: double*, a pointer to the vector with the gradient values
// Arg4: void*, a void pointer to the data structure
void grad_optim_objF(int n, double* args, double* grad, void* void_pointer_box) {
  // de-reference the void pointer to a pointer to optim_BOX instance
  optim_BOX *pointer_box = static_cast<optim_BOX*>(void_pointer_box);
  int index = (*pointer_box).index;
  NumericMatrix xy = (*pointer_box).xy;
  double x = xy(index,0);
  double y = xy(index,1);
  // update the values of the gradient
  grad[0] = log(x)*pow(x,args[0]) - log(2)*pow(2,-args[0]);
  grad[1] = log(y)*pow(y,args[1]) - log(2)*pow(2,-args[1]);
}


// [[Rcpp::export]]
List minimisation(NumericMatrix input) {

  const int n_functions = input.nrow();
  const int n_args = input.ncol();
  NumericMatrix args(n_functions,n_args);
  NumericVector min_Fx(n_functions);
  // put the data into the optim_BOX object
  optim_BOX box(input,-1);
  void* void_pointer_box = &box;

  // arguments for the vmmin C function
  // these exact data structures are enforced by the sygnature of the C function
  double *b = vect(n_args);
  double zero_zero = 0.0;
  double *Fmin;
  int maxit = 1000;
  int trace = 0;
  int *mask;
  mask = (int *) R_alloc(n_args, sizeof(int));
  double abstol = 1e-16;
  double reltol = 1e-16;
  int nREPORT = 10;
  int zero = 0;
  int *fncount;
  int *grcount;
  int *fail;

  // minimize for every (x_i,y_i) separately:
  for (int i = 0; i < n_functions; i++) {

    // reset/reassign some variables after every iteration of the loop
    // as some of them get modified.
    box.index = i;
    Fmin = &zero_zero;
    for (int j = 0; j < n_args; j++) mask[j] = 1;
    fncount = &zero;
    grcount = &zero;
    fail = &zero;
    // initiate starting points for the minimisation
    for (int j = 0; j < n_args; j++) b[j] = 0;

    // call the BFGS implemented in C
    // The sygnature: endpoints, values at endpoints, pointer to the objective function,
    // void pointer to the data, references to max_iter and eps variables
    vmmin(n_args, b, Fmin, *optim_objF, *grad_optim_objF, maxit, trace, mask,
          abstol, reltol, nREPORT, void_pointer_box, fncount, grcount, fail);

    //save the optimized results:
    min_Fx[i] = *Fmin;
    args(i,0) = b[0];
    args(i,1) = b[1];
  }

  // Create a named list to return the results into R in
  List L = List::create(Named("min_Fx")=min_Fx,_["args"]=args);
  return L;
}

// ##################################################################################


// ######################### INVERSE HESSIAN APPROXIMATION ##########################

// Objective function to calculate the hessian at
// Arg1 should be the argument of the function
// Arg2->N are free to define
double hessian_objF(Rcpp::NumericVector X, double C){
  double total = 0.0;
  for(int i = 0 ; i < X.size(); ++i) {
      total += pow(X[i], i+1);
  }
  return total*C;
}


// [[Rcpp::export]]
NumericMatrix inv_hessian(Rcpp::NumericVector arg, double C) {
  //load R functions
  Rcpp::Environment numDeriv("package:numDeriv");
  Rcpp::Function hessian = numDeriv["hessian"];
  Rcpp::Function cpp_solve("solve");
  //calculate the hessian of the objective function and its inverse
  const int len = arg.size();
  // In case of univariate objective function the data structures
  // are different and that is a special case.
  if (len==1){
    Rcpp::List hessian_results =
    hessian(
      Rcpp::_["func"] = Rcpp::InternalFunction(hessian_objF), 
      Rcpp::_["x"] = arg,
      Rcpp::_["C"] = C
    );
    double d2F_darg2 = hessian_results[0];
    // Inverse the 1x1 Hessian
    NumericMatrix inv_H(len,len);
    inv_H(0,0) = 1/d2F_darg2;
    return inv_H;
  }
  // multivariate objective function and NxN Hessian, N<1
  else {
    Rcpp::NumericMatrix hessian_results = 
    hessian(
      Rcpp::_["func"] = Rcpp::InternalFunction(hessian_objF),
      Rcpp::_["x"] = arg,
      Rcpp::_["C"] = C
    );
    // Inverse the Hessian
    NumericMatrix inv_H = cpp_solve(Named("a")=hessian_results);
    return inv_H;
  }
}

// ##################################################################################


