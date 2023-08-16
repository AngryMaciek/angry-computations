#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, char* argv[]){

    std::vector<float> data;
    std::string line;
    float temp;

    // construct PWD-independent path to the input file
    std::filesystem::path CWD = std::filesystem::current_path();
    std::filesystem::path EXECPATH = (CWD / argv[0]).parent_path();
    std::ifstream file(EXECPATH / "data/vector.txt");

    while (std::getline(file, line)){
        std::istringstream ss(line);
        if (ss >> temp) data.push_back(temp);
    }
    file.close();

    for (const float& value : data){
        std::cout << value << std::endl;
    }

    return 0;
}
