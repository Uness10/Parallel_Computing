#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void init_matrix(double *A, int n) {
    for (int i = 0; i < n * n; i++) {
        A[i] = (double)rand() / RAND_MAX;
    }
}

void zero_matrix(double *C, int n) {
    for (int i = 0; i < n * n; i++) {
        C[i] = 0.0;
    }
}

void block_matmul(double *A, double *B, double *C, int n, int Bsize) {
    for (int ii = 0; ii < n; ii += Bsize) {
        for (int jj = 0; jj < n; jj += Bsize) {
            for (int kk = 0; kk < n; kk += Bsize) {
                // For each block
                int i_max = (ii + Bsize < n) ? ii + Bsize : n;
                int j_max = (jj + Bsize < n) ? jj + Bsize : n;
                int k_max = (kk + Bsize < n) ? kk + Bsize : n;
                for (int i = ii; i < i_max; i++) {
                    for (int j = jj; j < j_max; j++) {
                        double sum = C[i * n + j];
                        for (int k = kk; k < k_max; k++) {
                            sum += A[i * n + k] * B[k * n + j];
                        }
                        C[i * n + j] = sum;
                    }
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int n = 1024; // Matrix size
    
    if (argc > 1) {
        n = atoi(argv[1]);
    }
    
    printf("Matrix size: %d x %d\n", n, n);
    printf("----------------------------------------\n");
    
    // Allocate matrices
    double *A = (double *)malloc(n * n * sizeof(double));
    double *B = (double *)malloc(n * n * sizeof(double));
    double *C = (double *)malloc(n * n * sizeof(double));
    
    if (!A || !B || !C) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    
    // Initialize matrices
    srand(42);
    init_matrix(A, n);
    init_matrix(B, n);
    

    // Calculate total operations for matrix multiplication: 2 * n^3 (multiply + add)
    double total_flops = 2.0 * (double)n * (double)n * (double)n;
    // Memory access: each element of A, B read once per block, C read/write
    // Approximate memory accessed: 3 * n^2 * sizeof(double) bytes minimum
    double total_memory_bytes = 3.0 * (double)n * (double)n * sizeof(double);
    printf("Block Size\tTime (s)\tMFLOPS\t\tBandwidth (MB/s)\n");
    printf("----------------------------------------------------------------\n");

    for (int block_size = 1; block_size <= n; block_size *=2) {

        zero_matrix(C, n);

        clock_t start = clock();
        block_matmul(A, B, C, n, block_size);
        clock_t end = clock();

        double time_spent = (double)(end - start) / CLOCKS_PER_SEC;

        double mflops = total_flops / (time_spent * 1e6);
        double bandwidth_gbs = total_memory_bytes / (time_spent * 1e6);

        printf("%d\t\t%.4f\t\t%.4f\t\t%.4f\n",
            block_size, time_spent, mflops, bandwidth_gbs);
    

    }
    

    // Cleanup
    free(A);
    free(B);
    free(C);
    
    return 0;
}