//
// Efficient statistical computing with C++ Armadillo
//
// Maciek Bak
// Swiss Institute of Bioinformatics
// 15.07.2019
//

#include <iostream>
#include <armadillo>

//=============================================================================

void armadillo_toy_examples(){

  // Generate a vector from the standard normal distribution
  arma::vec v_normal = arma::randn(5);
  v_normal.print("v_normal:");

  // Create a 4x4 random matrix and print it on the screen
  arma::Mat<double> A = arma::randu(4,4);
  std::cout << "A:\n" << A << "\n";

  // Create a new diagonal matrix using the main diagonal of A:
  arma::Mat<double>B = arma::diagmat(A);
  std::cout << "B:\n" << B << "\n";

  // New matrix: directly specify the matrix size (elements are uninitialised)
  arma::mat C(2,3);  // typedef mat  =  Mat<double>
  std::cout << "C.n_rows: " << C.n_rows << std::endl;
  std::cout << "C.n_cols: " << C.n_cols << std::endl;

  // Directly access an element (indexing starts at 0)
  C(1,2) = 456.0;
  std::cout << "C[1][2]:\n" << C(1,2) << "\n";
  C = 5.5; // scalars are treated as a 1x1 matrix
  C.print("C:");

  // Inverse
  std::cout << "inv(C): " << std::endl << inv(C) << std::endl;

  // Rotate a point (0,1) by -Pi/2 ---> (1,0)
  arma::vec Position = {0,1};
  Position.print("Current coordinates of a point:");
  double Pi = 3.14159265359;
  double phi = -Pi/2;
  arma::mat RotationMatrix = {
    {+cos(phi), -sin(phi)},
    {+sin(phi), +cos(phi)}
  };
  Position = RotationMatrix * Position;
  Position.print("New coordinates of the point:");
}

//=============================================================================

void armadillo_read_write_objects(){

  // Create a 5x5 matrix with random values from uniform distribution on [0;1]
  // Save a double matrix to a csv format, then load it.
  arma::Mat<double> uniform_matrix = arma::randu(3,5);
  uniform_matrix.save("data/arma_uniform_matrix.csv", arma::csv_ascii);
  arma::Mat<double> load_matrix;
  load_matrix.load("data/arma_uniform_matrix.csv", arma::csv_ascii);

  // Armadillo can save directly to files or write to pre-opened streams.
  // In order to add column names to output tables we have to write the
  // header manually to a file stream and then save the matrix to the stream.
  std::ofstream file("data/arma_uniform_matrix_with_headers.csv");
  file << "A,B,C,D,E" << std::endl;
  uniform_matrix.save(file, arma::csv_ascii);
  file.close();
  //
  // As Armadillo objects are numerical structures the input shall not contain
  // row/column names. Armadillo should be used only for heavy computations.
}

//=============================================================================

int main(int argc, const char **argv){

  std::cout << "Armadillo version: " << arma::arma_version::as_string()
    << std::endl;

  // set RNG seed:
  arma::arma_rng::set_seed(0);
  //arma::arma_rng::set_seed_random();

  armadillo_toy_examples();

  armadillo_read_write_objects();

  return 0;
}
