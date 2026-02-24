
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int value;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    do {
        if (rank == 0) {
            if (scanf("%d", &value) != 1) value = -1;
        }
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if (value >= 0) {
            printf("Rank %d received %d\n", rank, value);
            fflush(stdout);
        }
    } while (value >= 0);

    MPI_Finalize();
    return 0;
}