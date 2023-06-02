# https://gallery.rcpp.org/articles/eigen-eigenvalues

library(RcppEigen)

sourceCpp("RcppEigen.cpp")

set.seed(0)
X <- matrix(rnorm(4*4), 4, 4)
Z <- X %*% t(X)

# R function
print(eigen(Z))

# Eigen function
print(getEigenValues(Z))
