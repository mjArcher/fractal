#! /bin/bash
# clang++ -O3 src/main.cpp -std=c++11 -fopenmp=libiomp5 -lpng -lomp -o bin/fractal -I/usr/lib/gcc/x86_64-linux-gnu/4.8/include/
g++ -O3 src/main.cpp -lpng -Wall -std=c++11 -fopenmp -o bin/fractal 

