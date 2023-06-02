# Maciej Bak
# Swiss Institute of Bioinformatics
# 4.06.2019

suppressMessages(library(Rcpp)) # interface to cpp
suppressMessages(library(numDeriv)) # numeric hessian approximation
set.seed(0) # random numbers' seed

# add two numbers
cppFunction(
  "
  int add_two_numbers(int x, int y){
  int sum = x + y;
  return sum;
  }
  ")

# sum a vector of numbers
cppFunction(
  "
  double sum_vector(NumericVector x){
  int n = x.size();
  double total = 0;
  for(int i = 0; i < n; ++i) { total += x[i]; }
  return total;
  }
  ")

# calculate distance from a given center to the set of points
cppFunction(
  "
  NumericVector distance(double x, NumericVector ys){
  int n = ys.size();
  NumericVector out(n);
  for(int i = 0; i < n; ++i) {
    out[i] = sqrt(pow(ys[i] - x, 2.0));
  }
  return out;
  }
  ")

# sum rows of a numeric matrix
cppFunction(
  "
  NumericVector sum_rows(NumericMatrix x){
  int nrow = x.nrow(),
  ncol = x.ncol();
  NumericVector out(nrow);
  for (int i = 0; i < nrow; i++) {
    double total = 0;
    for (int j = 0; j < ncol; j++) {
      total += x(i, j);
    }
    out[i] = total;
  }
  return out;
  }
  ")

x = add_two_numbers(1, 2)
print(x)

x = sum_vector(c(1,2,4,5))
print(x)

x = distance(10,c(1,2,4,5))
print(x)

x = matrix(sample(100), 10)
print(sum_rows(x))

sourceCpp("tests.cpp")

### ROOT APROXIMATION FOR AN UNIVARIATE FUNCTION ###
#
# Let's say we have to find roots for a big amount of equations of the form:
# F(X) = 1/X + exp(X+1/x) - y
# for a given data series x,y.
# (any formula with no explicit analytical solution would be a good objective funciton)
# We will utilize the bisection method combined with Newton-Raphson iteration
# from R uniroot() function.
# In statistical modelling it is useful for finding roots of partial derrivatives of the
# Likelihood with respect to the parameters of the model.
x = sample(1:10, size=10, replace=TRUE)
y = sample(100:1000, size=10, replace=TRUE)
roots_C = calculate_roots(x,y)
print(roots_C)

### MINIMISATION OF A MULTIVARIATE FUNCTION ###
#
# Let's say we are given a multivariate function to be minimized:
# F(X1,X2) = x^X1 + 2^(-X1) + y^X2 + 2^(-X2)
# for a given data series x,y.
# (any complex enough formula would be a good objective funciton)
# We will utilize the BFGS method from R optim() function.
# In statistical modelling it is useful for minimisation of the negative Likelihood
# of the data given a model
x = sample(2:10, size=5, replace=TRUE)
y = sample(2:10, size=5, replace=TRUE)
xy = cbind(x,y)
minimisation_results = minimisation(xy)
colnames(minimisation_results$args) = c("x","y")
print(minimisation_results)

### INVERSE HESSIAN APPROXIMATION ###
#
# Let's say we are given a function and a point in which we want to
# evaluate the inverse of the hessian matrix:
# F(X_i,...) = C * SUM_i X_i^i
# for a given argument (X_i,...) and C in (0,1)
# We will utilise the hessian() function from R numDeriv package.
# In statistical modelling it is useful for estimation of the standard deviations
# of the parameters of the model (negative inverse of the hessian of the
# log-likelihood of the model at its optimum is the estimator of the asymptotic
# covariance matrix of the parameters).
C = 0.84
arg = c(1.0)
H = inv_hessian(arg,C)
print(H)
arg = c(1.0,1.0,1.0,1.0)
H = inv_hessian(arg,C)
print(H)
