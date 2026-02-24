#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void matrixVectorMultLocal(double* A_local, double* b, double* x_local, int local_rows, int N) {
    for (int i = 0; i < local_rows; ++i) {
        x_local[i] = 0.0;
        for (int j = 0; j < N; ++j) {
            x_local[i] += A_local[i * N + j] * b[j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <matrix_size>\n", argv[0]);
        return 1;
    }

    int N = atoi(argv[1]);
    if (N <= 0) {
        printf("Matrix size must be positive.\n");
        return 1;
    }

    MPI_Init(&argc, &argv);

    int rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int base_rows = N / world_size;
    int rem = N % world_size;

    int *sendcounts = malloc(world_size * sizeof(int));
    int *displs = malloc(world_size * sizeof(int));
    for (int r = 0, offset = 0; r < world_size; ++r) {
        int rows = base_rows + (r < rem ? 1 : 0);
        sendcounts[r] = rows * N; /* number of doubles sent to r */
        displs[r] = offset * N;
        offset += rows;
    }
    int local_rows = base_rows + (rank < rem ? 1 : 0);

    double *A = NULL;        /* full matrix only on root */
    double *b = malloc(N * sizeof(double));
    double *x_serial = NULL; /* only on root */
    double *x_parallel = NULL; /* only on root to gather results */

    if (rank == 0) {
        A = malloc((size_t)N * N * sizeof(double));
        x_serial = malloc(N * sizeof(double));
        x_parallel = malloc(N * sizeof(double));
        if (!A || !b || !x_serial || !x_parallel) {
            fprintf(stderr, "Memory allocation failed on root.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        srand48(42);

        /* Fill A[0][:100] with random values (adapted from original) */
        int limit = (N < 100) ? N : 100;
        for (int j = 0; j < limit; ++j) {
            A[0 * N + j] = drand48();
        }
        /* Copy into A[1][100:200] if possible */
        if (N > 1 && N > 100) {
            int copy_len = (N - 100 < 100) ? (N - 100) : 100;
            for (int j = 0; j < copy_len; ++j) {
                A[1 * N + (100 + j)] = A[0 * N + j];
            }
        }
        /* Set diagonal (and initialize other entries to 0 for safety) */
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == j) A[i * N + j] = drand48();
                else if (!(i == 0 && j < limit) && !(i == 1 && j >= 100 && j < 100 + (N - 100)))
                    A[i * N + j] = 0.0;
            }
        }
        /* Fill vector b */
        for (int i = 0; i < N; ++i) b[i] = drand48();

        /* Compute serial result for comparison and measure time */
        double t0 = MPI_Wtime();
        for (int i = 0; i < N; ++i) {
            x_serial[i] = 0.0;
            for (int j = 0; j < N; ++j) x_serial[i] += A[i * N + j] * b[j];
        }
        double t1 = MPI_Wtime();
        double t_serial = t1 - t0;

        /* Prepare recvcounts/rdispls for gather on root */
        int *recvcounts = malloc(world_size * sizeof(int));
        int *rdispls = malloc(world_size * sizeof(int));
        for (int r = 0, offset = 0; r < world_size; ++r) {
            int rows = base_rows + (r < rem ? 1 : 0);
            recvcounts[r] = rows;
            rdispls[r] = offset;
            offset += rows;
        }

        /* Barrier and time the parallel run (including communication) */
        MPI_Barrier(MPI_COMM_WORLD);
        double tp0 = MPI_Wtime();

        /* Broadcast b to all processes (included in timed region) */
        MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        /* Allocate local storage for rows (for root too) */
        double *A_local = NULL;
        if (local_rows > 0) {
            A_local = malloc((size_t)local_rows * N * sizeof(double));
            if (!A_local) {
                fprintf(stderr, "Rank %d: failed to allocate A_local\n", rank);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }

        /* Scatter rows of A (root provides send buffer) */
        MPI_Scatterv(
            A, sendcounts, displs, MPI_DOUBLE,
            A_local, local_rows * N, MPI_DOUBLE,
            0, MPI_COMM_WORLD
        );

        /* Local computation on root */
        double *x_local = NULL;
        if (local_rows > 0) {
            x_local = malloc(local_rows * sizeof(double));
            if (!x_local) {
                fprintf(stderr, "Rank %d: failed to allocate x_local\n", rank);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            matrixVectorMultLocal(A_local, b, x_local, local_rows, N);
        }

        /* Gather results to root */
        MPI_Gatherv(
            x_local, local_rows, MPI_DOUBLE,
            x_parallel, recvcounts, rdispls, MPI_DOUBLE,
            0, MPI_COMM_WORLD
        );

        MPI_Barrier(MPI_COMM_WORLD);
        double tp1 = MPI_Wtime();
        double t_parallel = tp1 - tp0;

        /* Gather max parallel time across processes to root (root already has t_parallel) */
        double max_parallel_time = t_parallel;
        MPI_Reduce(&t_parallel, &max_parallel_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        /* Compare parallel result with serial */
        double max_error = 0.0;
        for (int i = 0; i < N; ++i) {
            double diff = fabs(x_parallel[i] - x_serial[i]);
            if (diff > max_error) max_error = diff;
        }
        double speedup = t_serial / max_parallel_time;
        double efficiency = speedup / world_size;

        printf("N=%d P=%d serial=%.6e parallel(max)=%.6e speedup=%.6e efficiency=%.6e max_error=%e\n",
               N, world_size, t_serial, max_parallel_time, speedup, efficiency, max_error);

        /* Append results to CSV for plotting */
        FILE *f = fopen("timings.csv", "a");
        if (f) {
            fprintf(f, "%d,%d,%.12e,%.12e,%.12e,%.12e,%e\n",
                    N, world_size, t_serial, max_parallel_time, speedup, efficiency, max_error);
            fclose(f);
        }

        /* Cleanup root-local allocations */
        if (A_local) free(A_local);
        if (x_local) free(x_local);
        free(recvcounts);
        free(rdispls);
    } else {
        /* Non-root processes */

        /* Barrier and start timed parallel section on other ranks as well */
        MPI_Barrier(MPI_COMM_WORLD);
        double tp0 = MPI_Wtime();

        /* Receive broadcasted b */
        MPI_Bcast(b, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        /* Allocate local storage for rows */
        double *A_local = NULL;
        if (local_rows > 0) {
            A_local = malloc((size_t)local_rows * N * sizeof(double));
            if (!A_local) {
                fprintf(stderr, "Rank %d: failed to allocate A_local\n", rank);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
        }

        /* Receive scattered rows */
        MPI_Scatterv(
            NULL, sendcounts, displs, MPI_DOUBLE,
            A_local, local_rows * N, MPI_DOUBLE,
            0, MPI_COMM_WORLD
        );

        /* Local computation */
        double *x_local = NULL;
        if (local_rows > 0) {
            x_local = malloc(local_rows * sizeof(double));
            if (!x_local) {
                fprintf(stderr, "Rank %d: failed to allocate x_local\n", rank);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            matrixVectorMultLocal(A_local, b, x_local, local_rows, N);
        }

        /* Send local results to root */
        MPI_Gatherv(
            x_local, local_rows, MPI_DOUBLE,
            NULL, NULL, NULL, MPI_DOUBLE,
            0, MPI_COMM_WORLD
        );

        MPI_Barrier(MPI_COMM_WORLD);
        double tp1 = MPI_Wtime();
        double t_parallel = tp1 - tp0;

        /* Reduce to get max parallel time at root */
        MPI_Reduce(&t_parallel, NULL, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

        /* Cleanup non-root allocations */
        if (A_local) free(A_local);
        if (x_local) free(x_local);
    }

    /* Common cleanup */
    if (A) free(A);
    if (b) free(b);
    if (x_serial) free(x_serial);
    if (x_parallel) free(x_parallel);
    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}