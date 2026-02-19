#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000

void init_matrix(int n, double *A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            A[i*n + j] = (double)(i + j);
        }
    }
}

void print_matrix(int n, double *A) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%6.1f ", A[i*n + j]);
        }
        printf("\n");
    }
}

double sum_matrix(int n, double *A) {
    double sum = 0.0;
    for (int i = 0; i < n*n; i++) {
        sum += A[i];
    }
    return sum;
}

int main() {

    double *A;
    double sum;
    double start, end;

    A = (double*) malloc(N * N * sizeof(double));
    if (A == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    /* =============================================
     * VERSION 1: Sequential (without OpenMP)
     * ============================================= */
    printf("=== Sequential Version ===\n");

    start = omp_get_wtime();

    /* Initialization */
    init_matrix(N, A);

    /* Printing (commented because N is large) */
    /* print_matrix(N, A); */

    /* Sum computation */
    sum = sum_matrix(N, A);

    end = omp_get_wtime();

    printf("Sum = %lf\n", sum);
    printf("Execution time = %lf seconds\n", end - start);

    /* =============================================
     * VERSION 2: With OpenMP (master + single)
     * ============================================= */
    printf("\n=== OpenMP Version (master + single) ===\n");

    sum = 0.0;

    start = omp_get_wtime();

    #pragma omp parallel
    {
        /* Master thread initializes the matrix */
        #pragma omp master
        {
            init_matrix(N, A);
            printf("Matrix initialized by master thread (thread %d)\n",
                   omp_get_thread_num());
        }
        /* Implicit barrier not guaranteed after master, so we add one */
        #pragma omp barrier

        /* Single thread prints the matrix */
        #pragma omp single
        {
            /* print_matrix(N, A); */  /* Commented because N is large */
            printf("Matrix printing done by single thread (thread %d)\n",
                   omp_get_thread_num());
        }
        /* Implicit barrier after single */

        /* All threads compute the sum in parallel */
        double local_sum = 0.0;
        #pragma omp for reduction(+:sum)
        for (int i = 0; i < N * N; i++) {
            sum += A[i];
        }
    }

    end = omp_get_wtime();

    printf("Sum = %lf\n", sum);
    printf("Execution time = %lf seconds\n", end - start);
    printf("Number of threads used: %d\n", omp_get_max_threads());

    free(A);
    return 0;
}
