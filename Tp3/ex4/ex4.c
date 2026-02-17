#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <string.h>


int main() {
    int m = 800, n = 800;


    double *a = malloc((size_t)m * n * sizeof(double));
    double *b = malloc((size_t)n * m * sizeof(double));
    double *c_serial = malloc((size_t)m * m * sizeof(double));
    double *c_parallel = malloc((size_t)m * m * sizeof(double));

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            a[i * n + j] = (double)((i + 1) + (j + 1));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            b[i * m + j] = (double)((i + 1) - (j + 1));
    for (int i = 0; i < m; i++)
        for (int j = 0; j < m; j++) { c_serial[i * m + j] = 0.0; c_parallel[i * m + j] = 0.0; }

    fprintf(stderr, "Matrix multiply: m=%d n=%d max_threads=%d\n", m, n, omp_get_max_threads());

    /* Serial baseline */
    double t0 = omp_get_wtime();
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < m; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) sum += a[i * n + k] * b[k * m + j];
            c_serial[i * m + j] = sum;
        }
    }
    double t_serial = omp_get_wtime() - t0;
    fprintf(stderr, "Serial time: %f s\n", t_serial);

    int thread_counts[] = {1,2,4,8,16};
    int n_thread_counts = sizeof(thread_counts)/sizeof(thread_counts[0]);
    omp_sched_t scheds[] = {omp_sched_static, omp_sched_dynamic, omp_sched_guided};
    const char *sched_names[] = {"STATIC","DYNAMIC","GUIDED"};
    int n_scheds = sizeof(scheds)/sizeof(scheds[0]);
    int chunks[] = {1,4,16,64};
    int n_chunks = sizeof(chunks)/sizeof(chunks[0]);

    printf("threads,schedule,chunk,parallel_time,speedup,efficiency,verify\n");

    for (int si = 0; si < n_scheds; si++) {
        for (int ci = 0; ci < n_chunks; ci++) {
            int chunk = chunks[ci];
            omp_set_schedule(scheds[si], chunk);
            for (int ti = 0; ti < n_thread_counts; ti++) {
                int t = thread_counts[ti];
                omp_set_num_threads(t);

                int repeats = 3;
                double best = 1e300;
                int verify_ok = 1;
                for (int r = 0; r < repeats; r++) {
                    /* zero output buffer */
                    for (int i = 0; i < m * m; i++) c_parallel[i] = 0.0;
                    double tt0 = omp_get_wtime();
#pragma omp parallel for collapse(2) schedule(runtime)
                    for (int i = 0; i < m; i++) {
                        for (int j = 0; j < m; j++) {
                            double sum = 0.0;
                            for (int k = 0; k < n; k++) sum += a[i * n + k] * b[k * m + j];
                            c_parallel[i * m + j] = sum;
                        }
                    }
                    double tt = omp_get_wtime() - tt0;
                    if (tt < best) best = tt;
                    for (int i = 0; i < m && verify_ok; i++)
                        for (int j = 0; j < m; j++)
                            if (fabs(c_serial[i * m + j] - c_parallel[i * m + j]) > 1e-6) { verify_ok = 0; break; }
                }

                double speedup = t_serial / best;
                double efficiency = speedup / (double)t;
                printf("%d,%s,%d,%.6f,%.6f,%.6f,%s\n", t, sched_names[si], chunk, best, speedup, efficiency, verify_ok ? "OK" : "MISMATCH");
                fflush(stdout);
            }
        }
    }

    free(a); free(b); free(c_serial); free(c_parallel);
    return 0;
}
