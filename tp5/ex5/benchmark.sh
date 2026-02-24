#!/bin/bash

mpicc -O3 ex5.c -o ex5

rm -f timings.csv

for N in 1000000 10000000 100000000; do
    for P in 1 2 4 8 16; do
        mpirun -np $P ./ex5 $N
    done
done
