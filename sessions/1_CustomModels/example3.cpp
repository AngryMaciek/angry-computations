#include <iostream>
#include <vector>
#include "rapidcsv.h"

int main(int argc, char* argv[]){

    // construct PWD-independent path to the input file
    std::filesystem::path CWD = std::filesystem::current_path();
    std::filesystem::path EXECPATH = (CWD / argv[0]).parent_path();
    rapidcsv::Document doc(
        EXECPATH / "data/matrix.csv",
        rapidcsv::LabelParams(-1, -1),
        rapidcsv::SeparatorParams(',')
    );

    int numRows = doc.GetRowCount();
    int numColumns = doc.GetColumnCount();

    float** data = new float*[numRows];
    for (int row = 0; row < numRows; ++row) data[row] = new float[numColumns];

    for (int row=0; row<numRows; ++row)
        for (int col=0; col<numColumns; ++col)
            data[row][col] = doc.GetCell<float>(col, row);

    for (int row = 0; row < numRows; ++row) delete[] data[row];
    delete[] data;

    for (int i=0; i<numColumns; ++i){
        std::vector<float> col = doc.GetColumn<float>(i);
        for (const float& value : col) std::cout << value << std::endl;
        std::cout << "===" << std::endl;

    }

    return 0;
}
