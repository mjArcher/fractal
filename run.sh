#! /bin/bash
# export OMP_NUM_THREADS=$1
echo "num threads bash"$1
time ./bin/fractal $1
