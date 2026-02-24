#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    long long N = 1000000; // Default value
    if (argc > 1) {
        N = atoll(argv[1]);
    }

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Synchronize before starting the timer
    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    long long base_iters = N / size;
    long long rem = N % size;

    long long local_iters = base_iters + (rank < rem ? 1 : 0);
    long long start_i = rank * base_iters + (rank < rem ? rank : rem);
    long long end_i = start_i + local_iters;

    double local_sum = 0.0;
    for (long long i = start_i; i < end_i; i++) {
        double x = (i + 0.5) / N;
        local_sum += 1.0 / (1.0 + x * x);
    }

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double t1 = MPI_Wtime();
    double t_parallel = t1 - t0;

    // Get the maximum parallel time across all processes
    double max_parallel_time = t_parallel;
    MPI_Reduce(&t_parallel, &max_parallel_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 4.0 / N * global_sum;
        
        // Compute serial time for speedup calculation
        double t2 = MPI_Wtime();
        double serial_sum = 0.0;
        for (long long i = 0; i < N; i++) {
            double x = (i + 0.5) / N;
            serial_sum += 1.0 / (1.0 + x * x);
        }
        double serial_pi = 4.0 / N * serial_sum;
        double t3 = MPI_Wtime();
        double t_serial = t3 - t2;

        double speedup = t_serial / max_parallel_time;
        double efficiency = speedup / size;

        printf("N=%lld P=%d pi=%.15f error=%.15e serial_pi=%.15f serial=%.6e parallel=%.6e speedup=%.6e efficiency=%.6e\n",
               N, size, pi, pi - 3.14159265358979323846, serial_pi, t_serial, max_parallel_time, speedup, efficiency);

        FILE *f = fopen("timings.csv", "a");
        if (f) {
            fprintf(f, "%lld,%d,%.12e,%.12e,%.12e,%.12e\n",
                    N, size, t_serial, max_parallel_time, speedup, efficiency);
            fclose(f);
        }
    }

    MPI_Finalize();
    return 0;
}
