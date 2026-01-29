#!/bin/bash

Ns=(1000 5000 10000 20000)
NBs=(1 2 4 8 16 32 64 128 256)

OUT="results.txt"
echo "N NB time(s) GFLOPS Status" > $OUT

for N in "${Ns[@]}"; do
  for NB in "${NBs[@]}"; do
    echo "Running HPL: N=$N NB=$NB"
    sed -i "6s/.*/$N         Ns/" HPL.dat
    sed -i "8s/.*/$NB         NBs/" HPL.dat
    srun --mpi=pmix -n 1 ./xhpl > hpl.out
    TIME=$(grep WR00C2R4 hpl.out | awk '{print $(NF-1)}')
    GFLOPS=$(grep WR00C2R4 hpl.out | awk '{print $NF}')
    STATUS=$(grep WR00C2R4 hpl.out | awk '{print $(NF-2)}')

    echo "$N $NB $TIME $GFLOPS $STATUS" | tee -a $OUT

  done
done