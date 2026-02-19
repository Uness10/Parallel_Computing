#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define N 1000000

int main() {
    double *A = malloc(N * sizeof(double));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    double sum = 0.0;
    double mean = 0.0;
    double stddev = 0.0;
    double max;
    double start, end;

    // Initialization
    srand(0);
    for (int i = 0; i < N; i++)
        A[i] = (double)rand() / RAND_MAX;

    /* =============================================
     * VERSION 1: Sequential
     * ============================================= */
    printf("=== Sequential Version ===\n");
    start = omp_get_wtime();

    // Compute sum
    sum = 0.0;
    for (int i = 0; i < N; i++)
        sum += A[i];

    // Compute max
    max = A[0];
    for (int i = 0; i < N; i++) {
        if (A[i] > max)
            max = A[i];
    }

    // Compute mean and standard deviation
    mean = sum / N;
    stddev = 0.0;
    for (int i = 0; i < N; i++)
        stddev += (A[i] - mean) * (A[i] - mean);
    stddev = sqrt(stddev / N);

    end = omp_get_wtime();

    printf("Sum     = %f\n", sum);
    printf("Max     = %f\n", max);
    printf("Std Dev = %f\n", stddev);
    printf("Time    = %f seconds\n", end - start);

    /* =============================================
     * VERSION 2: Parallel Sections with dependency
     * Section 3 (stddev) depends on Section 1 (sum)
     * We use a volatile flag for synchronization.
     * ============================================= */
    printf("\n=== OpenMP Parallel Sections Version ===\n");

    double par_sum = 0.0;
    double par_max;
    double par_mean = 0.0;
    double par_stddev = 0.0;
    volatile int sum_done = 0;

    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        /* Section 1: Compute the sum */
        #pragma omp section
        {
            printf("Section 1 (sum)    -> thread %d\n", omp_get_thread_num());
            par_sum = 0.0;
            for (int i = 0; i < N; i++)
                par_sum += A[i];
            sum_done = 1;  /* Signal that sum is ready */
        }

        /* Section 2: Compute the maximum */
        #pragma omp section
        {
            printf("Section 2 (max)    -> thread %d\n", omp_get_thread_num());
            par_max = A[0];
            for (int i = 0; i < N; i++) {
                if (A[i] > par_max)
                    par_max = A[i];
            }
        }

        /* Section 3: Compute the standard deviation (needs sum from Section 1) */
        #pragma omp section
        {
            printf("Section 3 (stddev) -> thread %d\n", omp_get_thread_num());
            /* Busy-wait until Section 1 has finished computing the sum */
            while (!sum_done) {
                /* spin */
            }
            par_mean = par_sum / N;
            par_stddev = 0.0;
            for (int i = 0; i < N; i++)
                par_stddev += (A[i] - par_mean) * (A[i] - par_mean);
            par_stddev = sqrt(par_stddev / N);
        }
    }

    end = omp_get_wtime();

    printf("Sum     = %f\n", par_sum);
    printf("Max     = %f\n", par_max);
    printf("Std Dev = %f\n", par_stddev);
    printf("Time    = %f seconds\n", end - start);

    /* Verify correctness */
    printf("\n=== Verification ===\n");
    printf("Sum     match: %s\n", (fabs(sum - par_sum) < 1e-6) ? "OK" : "MISMATCH");
    printf("Max     match: %s\n", (fabs(max - par_max) < 1e-6) ? "OK" : "MISMATCH");
    printf("Std Dev match: %s\n", (fabs(stddev - par_stddev) < 1e-6) ? "OK" : "MISMATCH");

    free(A);
    return 0;
}
