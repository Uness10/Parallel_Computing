#!/usr/bin/env bash
set -euo pipefail

sizes=(32 64 128 512 1024)
procs=(1 2 4 8 16)
exe="./ex4"

if [[ ! -x $exe ]]; then
    echo "Executable $exe not found or not executable" >&2
    exit 1
fi

for N in "${sizes[@]}"; do
    for p in "${procs[@]}"; do
        echo "Running: N=$N p=$p"
        mpirun -np "$p" "$exe" "$N"
    done
done