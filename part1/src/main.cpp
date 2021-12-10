#include "MarchingCubes.hpp"

int main(int argv, char** args) {
    float data[] = {
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        
        0, 0, 0,
        0, 1, 0,
        0, 0, 0,

        0, 0, 0,
        0, 0, 0,
        0, 0, 0
    };
    MarchingCubes mc(3, 3, 3, 3, 3, 3, data, 0.5f);
}