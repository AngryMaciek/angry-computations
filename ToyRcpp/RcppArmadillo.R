# https://gallery.rcpp.org/articles/armadillo-eigenvalues

library(Rcpp)

sourceCpp("RcppArmadillo.cpp")

set.seed(0)
X <- matrix(rnorm(4*4), 4, 4)
Z <- X %*% t(X)

# R function
print(eigen(Z))

# Eigen function
print(getEigenValues(Z))
