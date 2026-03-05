#!/bin/bash
#SBATCH --job-name=gradient_scaling
#SBATCH --output=scaling_%j.out
#SBATCH --error=scaling_%j.err
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=56
#SBATCH --time=00:30:00
#SBATCH --partition=compute

# Load MPI module
module load OpenMPI/4.1.6-GCC-13.2.0

# Compile the program
mpicc -Wall -O2 -o distrib_grad distrib_grad.c -lm

# Results file
RESULTS_FILE="scaling_results.csv"
echo "processes,time,samples" > $RESULTS_FILE

# Array of process counts to test
PROCS=(1 2 4 7 8 14 16 28 32 48 56)

# Number of samples to test (can be modified)
N_SAMPLES=100000

# Modify the source to use more samples for better scaling tests
sed -i "s/#define N_SAMPLES.*/#define N_SAMPLES ${N_SAMPLES}/" distrib_grad.c
mpicc -Wall -O2 -o distrib_grad distrib_grad.c -lm

# Run scaling experiments
for np in "${PROCS[@]}"; do
    echo "Running with $np processes..."
    
    # Run and capture timing
    output=$(mpirun -np $np ./distrib_grad 2>&1)
    
    # Extract time from output
    time=$(echo "$output" | grep "Training time" | awk '{print $3}')
    
    # Save to results
    echo "$np,$time,$N_SAMPLES" >> $RESULTS_FILE
    
    echo "  Time: $time seconds"
done

echo ""
echo "Scaling results saved to $RESULTS_FILE"
echo ""
cat $RESULTS_FILE
