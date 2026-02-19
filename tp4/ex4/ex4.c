#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/* Dense Matrix-Vector Multiplication: lhs = mat * rhs
 * mat is m x n (stored column-major: mat[r + c*m])
 * rhs is n x 1
 * lhs is m x 1
 */

/* Version 1: Implicit barrier (default) */
void dmvm_v1_implicit_barrier(int n, int m,
    double *lhs, double *rhs, double *mat)
{
    #pragma omp parallel for schedule(static)
    for (int r = 0; r < m; ++r) {
        double sum = 0.0;
        for (int c = 0; c < n; ++c)
            sum += mat[r + c * m] * rhs[c];
        lhs[r] = sum;
    }
    /* Implicit barrier at end of parallel for */
}

/* Version 2: schedule(dynamic) with nowait */
void dmvm_v2_dynamic_nowait(int n, int m,
    double *lhs, double *rhs, double *mat)
{
    #pragma omp parallel
    {
        #pragma omp for schedule(dynamic) nowait
        for (int r = 0; r < m; ++r) {
            double sum = 0.0;
            for (int c = 0; c < n; ++c)
                sum += mat[r + c * m] * rhs[c];
            lhs[r] = sum;
        }
        /* No barrier here due to nowait */
    }
}

/* Version 3: schedule(static) with nowait */
void dmvm_v3_static_nowait(int n, int m,
    double *lhs, double *rhs, double *mat)
{
    #pragma omp parallel
    {
        #pragma omp for schedule(static) nowait
        for (int r = 0; r < m; ++r) {
            double sum = 0.0;
            for (int c = 0; c < n; ++c)
                sum += mat[r + c * m] * rhs[c];
            lhs[r] = sum;
        }
        /* No barrier here due to nowait */
    }
}

/* Sequential version for reference */
void dmvm_sequential(int n, int m,
    double *lhs, double *rhs, double *mat)
{
    for (int r = 0; r < m; ++r) {
        double sum = 0.0;
        for (int c = 0; c < n; ++c)
            sum += mat[r + c * m] * rhs[c];
        lhs[r] = sum;
    }
}

/* Verify results */
int verify(int m, double *lhs, double expected) {
    for (int r = 0; r < m; ++r) {
        if (lhs[r] != expected) {
            printf("Verification FAILED at lhs[%d] = %.1f (expected %.1f)\n",
                   r, lhs[r], expected);
            return 0;
        }
    }
    return 1;
}

int main(void)
{
    const int n = 40000; // columns
    const int m = 600;   // rows
    const int num_runs = 5; // average over multiple runs

    /* FLOPs: for each of m rows, we do n multiplications + n additions = 2*m*n */
    double flops = 2.0 * m * n;

    double *mat = malloc(n * m * sizeof(double));
    double *rhs = malloc(n * sizeof(double));
    double *lhs = malloc(m * sizeof(double));

    if (!mat || !rhs || !lhs) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Initialization
    for (int c = 0; c < n; ++c) {
        rhs[c] = 1.0;
        for (int r = 0; r < m; ++r)
            mat[r + c * m] = 1.0;
    }

    double expected = (double)n; // each lhs[r] = sum of n * 1.0 * 1.0 = n

    printf("Matrix-Vector Multiplication: mat[%d x %d] * rhs[%d]\n", m, n, n);
    printf("FLOPs per run: %.0f\n", flops);
    printf("Number of runs: %d\n\n", num_runs);

    /* Get number of threads */
    int num_threads;
    #pragma omp parallel
    {
        #pragma omp single
        num_threads = omp_get_num_threads();
    }
    printf("Number of threads: %d\n\n", num_threads);

    /* =============================================
     * Sequential Version
     * ============================================= */
    for (int r = 0; r < m; ++r) lhs[r] = 0.0;
    double t_seq_start = omp_get_wtime();
    for (int run = 0; run < num_runs; ++run) {
        for (int r = 0; r < m; ++r) lhs[r] = 0.0;
        dmvm_sequential(n, m, lhs, rhs, mat);
    }
    double t_seq = (omp_get_wtime() - t_seq_start) / num_runs;
    verify(m, lhs, expected);
    printf("%-35s : Time = %.6f s | MFLOP/s = %.2f\n",
           "Sequential", t_seq, flops / (t_seq * 1e6));

    /* =============================================
     * Version 1: Implicit barrier
     * ============================================= */
    double t_v1_start = omp_get_wtime();
    for (int run = 0; run < num_runs; ++run) {
        for (int r = 0; r < m; ++r) lhs[r] = 0.0;
        dmvm_v1_implicit_barrier(n, m, lhs, rhs, mat);
    }
    double t_v1 = (omp_get_wtime() - t_v1_start) / num_runs;
    verify(m, lhs, expected);
    double speedup_v1 = t_seq / t_v1;
    double efficiency_v1 = speedup_v1 / num_threads;
    printf("%-35s : Time = %.6f s | Speedup = %.2f | Efficiency = %.2f | MFLOP/s = %.2f\n",
           "V1 - Implicit barrier (static)", t_v1, speedup_v1, efficiency_v1,
           flops / (t_v1 * 1e6));

    /* =============================================
     * Version 2: dynamic + nowait
     * ============================================= */
    double t_v2_start = omp_get_wtime();
    for (int run = 0; run < num_runs; ++run) {
        for (int r = 0; r < m; ++r) lhs[r] = 0.0;
        dmvm_v2_dynamic_nowait(n, m, lhs, rhs, mat);
    }
    double t_v2 = (omp_get_wtime() - t_v2_start) / num_runs;
    verify(m, lhs, expected);
    double speedup_v2 = t_seq / t_v2;
    double efficiency_v2 = speedup_v2 / num_threads;
    printf("%-35s : Time = %.6f s | Speedup = %.2f | Efficiency = %.2f | MFLOP/s = %.2f\n",
           "V2 - dynamic + nowait", t_v2, speedup_v2, efficiency_v2,
           flops / (t_v2 * 1e6));

    /* =============================================
     * Version 3: static + nowait
     * ============================================= */
    double t_v3_start = omp_get_wtime();
    for (int run = 0; run < num_runs; ++run) {
        for (int r = 0; r < m; ++r) lhs[r] = 0.0;
        dmvm_v3_static_nowait(n, m, lhs, rhs, mat);
    }
    double t_v3 = (omp_get_wtime() - t_v3_start) / num_runs;
    verify(m, lhs, expected);
    double speedup_v3 = t_seq / t_v3;
    double efficiency_v3 = speedup_v3 / num_threads;
    printf("%-35s : Time = %.6f s | Speedup = %.2f | Efficiency = %.2f | MFLOP/s = %.2f\n",
           "V3 - static + nowait", t_v3, speedup_v3, efficiency_v3,
           flops / (t_v3 * 1e6));

    /* =============================================
     * Summary
     * ============================================= */
    printf("\n=== Summary ===\n");
    printf("%-35s | %-12s | %-10s | %-12s | %-12s\n",
           "Version", "Time (s)", "Speedup", "Efficiency", "MFLOP/s");
    printf("--------------------------------------------------------------------------"
           "-------------------\n");
    printf("%-35s | %12.6f | %10s | %12s | %12.2f\n",
           "Sequential", t_seq, "1.00", "1.00", flops / (t_seq * 1e6));
    printf("%-35s | %12.6f | %10.2f | %12.2f | %12.2f\n",
           "V1 - Implicit barrier (static)", t_v1, speedup_v1, efficiency_v1,
           flops / (t_v1 * 1e6));
    printf("%-35s | %12.6f | %10.2f | %12.2f | %12.2f\n",
           "V2 - dynamic + nowait", t_v2, speedup_v2, efficiency_v2,
           flops / (t_v2 * 1e6));
    printf("%-35s | %12.6f | %10.2f | %12.2f | %12.2f\n",
           "V3 - static + nowait", t_v3, speedup_v3, efficiency_v3,
           flops / (t_v3 * 1e6));

    printf("\n=== Analysis ===\n");
    printf("Why barriers limit scalability:\n");
    printf("  - All threads must wait for the slowest thread to finish before proceeding.\n");
    printf("  - This idle time (synchronization overhead) increases with thread count.\n");
    printf("  - With unbalanced workloads, faster threads waste cycles waiting.\n\n");
    printf("When nowait becomes dangerous:\n");
    printf("  - When subsequent code depends on the results of the parallel loop.\n");
    printf("  - If threads read/write shared data after the loop without synchronization.\n");
    printf("  - Can cause race conditions and incorrect results.\n");

    free(mat);
    free(rhs);
    free(lhs);
    return 0;
}
