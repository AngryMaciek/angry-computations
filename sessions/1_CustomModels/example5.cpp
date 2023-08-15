#include <iostream>
#include <boost/math/differentiation/autodiff.hpp>
#include <boost/math/tools/minima.hpp>

template <typename T>
T x4(T const& x){
  T x4 = x * x;
  x4 *= x4;
  return x4;
}

struct F{
  double operator()(double const& x){
    return (x + 3) * (x - 1) * (x - 1);
  }
};

int main(){

  // automatic differentiation
  constexpr unsigned ord = 5;
  double arg = 0.1;
  auto const x = boost::math::differentiation::make_fvar<double, ord>(arg);
  auto const y = x4(x);
  for (unsigned i=0; i<=ord; ++i)
    std::cout <<
      "d(" << i << ")/dx [x^4] | x=" << arg << " | = " << y.derivative(i)
      << std::endl;

  std::cout << "=====" << std::endl;

  // mimimum search
  int bits = std::numeric_limits<double>::digits;
  double lower = -4.0;
  double upper = 4.0 / 3;
  std::pair<double, double> r = boost::math::tools::brent_find_minima(
    F(), lower, upper, bits
  );
  std::cout.precision(std::numeric_limits<double>::digits10);
  std::cout << "   min[F(x)] = " << r.second << std::endl;
  std::cout << "minarg[F(x)] = " << r.first << std::endl;

  return 0;
}
