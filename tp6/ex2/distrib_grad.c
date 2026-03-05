/**
 * Exercise 2: Distributed Gradient Descent with MPI Derived Types
 * 
 * This program implements a distributed batch gradient descent algorithm
 * using MPI. It uses MPI_Type_create_struct to define a custom datatype
 * for exchanging training samples (features + label) across processes.
 * 
 * Compile: mpicc -o distrib_grad distrib_grad.c -lm
 * Run: mpirun -np 4 ./distrib_grad
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <mpi.h>

#define N_FEATURES 2
#define N_SAMPLES 100000
#define LEARNING_RATE 0.01
#define MAX_EPOCHS 10000
#define MSE_THRESHOLD 0.01
#define PRINT_INTERVAL 10

// Sample structure: features vector + label
typedef struct {
    double x[N_FEATURES];
    double y;
} Sample;

// Generate synthetic dataset: y = w0*x0 + w1*x1 + noise
void generate_data(Sample *data, int n_samples, double *true_weights) {
    srand(42);  // Fixed seed for reproducibility
    
    for (int i = 0; i < n_samples; i++) {
        data[i].y = 0.0;
        for (int j = 0; j < N_FEATURES; j++) {
            data[i].x[j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0;  // [-1, 1]
            data[i].y += true_weights[j] * data[i].x[j];
        }
        // Add some noise
        data[i].y += ((double)rand() / RAND_MAX) * 0.2 - 0.1;
    }
}

// Compute prediction: y_pred = w . x
double predict(double *weights, double *x) {
    double pred = 0.0;
    for (int j = 0; j < N_FEATURES; j++) {
        pred += weights[j] * x[j];
    }
    return pred;
}

// Create MPI derived type for Sample structure
MPI_Datatype create_sample_type() {
    MPI_Datatype sample_type;
    
    // Define block lengths: N_FEATURES doubles for x, 1 double for y
    int block_lengths[2] = {N_FEATURES, 1};
    
    // Define displacements (byte offsets from start of structure)
    MPI_Aint displacements[2];
    Sample sample = {{0}, 0};
    MPI_Aint base_address;
    MPI_Get_address(&sample, &base_address);
    MPI_Get_address(&sample.x[0], &displacements[0]);
    MPI_Get_address(&sample.y, &displacements[1]);
    displacements[0] -= base_address;
    displacements[1] -= base_address;
    
    // Define types for each block
    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    
    // Create the struct type
    MPI_Type_create_struct(2, block_lengths, displacements, types, &sample_type);
    MPI_Type_commit(&sample_type);
    
    return sample_type;
}

int main(int argc, char *argv[]) {
    int rank, size;
    double start_time, end_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Step 1: Create derived type for Sample struct
    MPI_Datatype sample_type = create_sample_type();
    
    // True weights (unknown to algorithm, used for data generation)
    double true_weights[N_FEATURES] = {2.0, -1.0};
    
    // Full dataset (only used on process 0)
    Sample *full_data = NULL;
    
    // Calculate samples per process
    int base_count = N_SAMPLES / size;
    int remainder = N_SAMPLES % size;
    
    // Prepare scatter counts and displacements
    int *send_counts = malloc(size * sizeof(int));
    int *displs = malloc(size * sizeof(int));
    
    int offset = 0;
    for (int i = 0; i < size; i++) {
        send_counts[i] = base_count + (i < remainder ? 1 : 0);
        displs[i] = offset;
        offset += send_counts[i];
    }
    
    int local_count = send_counts[rank];
    Sample *local_data = malloc(local_count * sizeof(Sample));
    
    // Step 2: Generate full dataset on process 0
    if (rank == 0) {
        full_data = malloc(N_SAMPLES * sizeof(Sample));
        generate_data(full_data, N_SAMPLES, true_weights);
        printf("Training with %d samples distributed across %d processes\n", N_SAMPLES, size);
        printf("True weights: w[0]=%.4f, w[1]=%.4f\n\n", true_weights[0], true_weights[1]);
    }
    
    start_time = MPI_Wtime();
    
    // Step 3: Scatter dataset to all processes using MPI_Scatterv
    MPI_Scatterv(full_data, send_counts, displs, sample_type,
                 local_data, local_count, sample_type,
                 0, MPI_COMM_WORLD);
    
    // Initialize weights (same on all processes)
    double weights[N_FEATURES] = {0.0};
    
    int epoch;
    double global_loss = 0.0;
    
    for (epoch = 0; epoch < MAX_EPOCHS; epoch++) {
        // Step 4: Compute local loss and gradient
        double local_loss = 0.0;
        double local_gradient[N_FEATURES] = {0.0};
        
        for (int i = 0; i < local_count; i++) {
            double pred = predict(weights, local_data[i].x);
            double error = pred - local_data[i].y;
            
            // Accumulate MSE loss
            local_loss += error * error;
            
            // Accumulate gradient: d(MSE)/dw_j = 2 * error * x_j
            for (int j = 0; j < N_FEATURES; j++) {
                local_gradient[j] += 2.0 * error * local_data[i].x[j];
            }
        }
        
        // Step 5: Aggregate gradients and losses across all processes
        double global_gradient[N_FEATURES];
        MPI_Allreduce(local_gradient, global_gradient, N_FEATURES, 
                      MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        MPI_Allreduce(&local_loss, &global_loss, 1, 
                      MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        
        // Compute average loss (MSE)
        global_loss /= N_SAMPLES;
        
        // Step 6: Update weights (same update on all processes)
        for (int j = 0; j < N_FEATURES; j++) {
            weights[j] -= LEARNING_RATE * global_gradient[j] / N_SAMPLES;
        }
        
        // Step 7: Print loss every PRINT_INTERVAL epochs from process 0
        if (rank == 0 && (epoch + 1) % PRINT_INTERVAL == 0) {
            printf("Epoch %d | Loss (MSE): %.6f | w[0]: %.4f, w[1]: %.4f\n",
                   epoch + 1, global_loss, weights[0], weights[1]);
        }
        
        // Step 8: Early stopping if loss < threshold
        if (global_loss < MSE_THRESHOLD) {
            if (rank == 0) {
                printf("\nEarly stopping at epoch %d — loss %.6f < %.1e\n",
                       epoch + 1, global_loss, MSE_THRESHOLD);
            }
            break;
        }
    }
    
    end_time = MPI_Wtime();
    
    // Final output
    if (rank == 0) {
        printf("\nFinal weights: w[0]=%.4f, w[1]=%.4f\n", weights[0], weights[1]);
        printf("Training time: %.3f seconds (MPI with %d processes)\n", 
               end_time - start_time, size);
        free(full_data);
    }
    
    free(local_data);
    free(send_counts);
    free(displs);
    MPI_Type_free(&sample_type);
    
    MPI_Finalize();
    return 0;
}
