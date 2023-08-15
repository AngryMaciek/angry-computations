//
// Efficient statistical computing with C++ Eigen
//
// Maciek Bak
// Swiss Institute of Bioinformatics
// 29.07.2019
//

#include <iostream>
#include <ctime>
#include "Eigen/Dense"

using namespace Eigen;

//=============================================================================

void eigen_toy_examples(){

  // Generate a vector from the standard normal distribution
  VectorXd v_normal = VectorXd::Random(5);
  std::cout << "v_normal:" << std::endl;
  std::cout << v_normal << std::endl;

  // Create a 4x4 random matrix and print it on the screen
  MatrixXd A = MatrixXd::Random(4, 4);
  std::cout << "A:" << std::endl;
  std::cout << A << std::endl;

  // Rotate a point (0,1) by -Pi/2 ---> (1,0)
  Vector2d Position(0, 1);
  std::cout << "Current coordinates of a point:" << std::endl;
  std::cout << Position << std::endl;
  double Pi = 3.14159265359;
  double phi = -Pi/2;
  // Rotation Matrix:
  Matrix2d RotationMatrix;
  RotationMatrix << +cos(phi), -sin(phi), +sin(phi), +cos(phi);
  Position = RotationMatrix * Position;
  std::cout << "New coordinates of the point:" << std::endl;
  std::cout << Position << std::endl;

}

//=============================================================================

int main(int argc, const char **argv){

  std::cout
    << "Eigen version: "
    << EIGEN_WORLD_VERSION
    << "."
    << EIGEN_MAJOR_VERSION
    << "."
    << EIGEN_MINOR_VERSION
    << std::endl;

  // set RNG seed:
  std::srand(0);

  eigen_toy_examples();

  return 0;
}
