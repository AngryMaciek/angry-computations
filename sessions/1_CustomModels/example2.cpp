#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]){

    std::vector<std::vector<float>> data;
    std::string line;
    float temp;

    // construct PWD-independent path to the input file
    std::filesystem::path CWD = std::filesystem::current_path();
    std::filesystem::path EXECPATH = (CWD / argv[0]).parent_path();
    std::ifstream file(EXECPATH / "data/matrix.csv");

    while (std::getline(file, line)){
        std::istringstream ss(line);
        std::vector<float> row;
        while (ss >> temp){
            row.push_back(temp);
            if (ss.peek() == ',') ss.ignore();
        }
        data.push_back(row);
    }
    file.close();

    for (const std::vector<float>& row : data){
        for (int i=0; i<row.size(); ++i){
            std::cout << row[i];
            if (i!=row.size()-1) std::cout << ",";
        }
        std::cout << std::endl;
    }

    return 0;
}
