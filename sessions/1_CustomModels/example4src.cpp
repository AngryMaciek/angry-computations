#include <cmath>
#include <cstdlib>
#include <ctime>

float F(float x){
    return (2*x + pow(2,x)) / x;
}

int G(int seed=time(nullptr)){
    srand(seed);
    return 1 + std::rand() % 100;
}
