/**
 * Exercise 1: Matrix Transposition using MPI Derived Types
 * 
 * This program demonstrates how to transpose a matrix during MPI communication
 * using MPI_Type_vector and MPI_Type_create_hvector derived datatypes.
 * 
 * Compile: mpicc -o matrix_transpose matrix_transpose.c
 * Run: mpirun -np 2 ./matrix_transpose
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ROWS 4
#define COLS 5

int main(int argc, char *argv[]) {
    int rank, size;
    double a[ROWS][COLS];      // Original matrix on process 0
    double at[COLS][ROWS];     // Transposed matrix on process 1
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (size != 2) {
        if (rank == 0) {
            printf("This program requires exactly 2 processes.\n");
        }
        MPI_Finalize();
        return 1;
    }
    
    if (rank == 0) {
        // Step 1: Initialize matrix a[4][5] with values 1 to 20
        int val = 1;
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                a[i][j] = val++;
            }
        }
        
        // Step 2: Display matrix a on process 0 before sending
        printf("Process 0 - Matrix a:\n");
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                printf("%4.0f ", a[i][j]);
            }
            printf("\n");
        }
        printf("\n");
        
        // Step 6: Send the original matrix with regular MPI_Send
        // We send all ROWS*COLS elements as contiguous data
        MPI_Send(&a[0][0], ROWS * COLS, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
    }
    else if (rank == 1) {
        // Step 3: Declare at[5][4] to hold the transposed result
        // Initialize to zero
        for (int i = 0; i < COLS; i++) {
            for (int j = 0; j < ROWS; j++) {
                at[i][j] = 0.0;
            }
        }
        
        // Step 4: Build derived datatype for receiving transposed data
        // Source: 4x5 matrix sent as 20 contiguous elements (row-major)
        // Destination: 5x4 matrix where each source row becomes a destination column
        
        MPI_Datatype column_type, transpose_type;
        
        // column_type: places COLS (5) elements with stride ROWS (4) in destination
        // This scatters one source row into one destination column
        // count=COLS (5 elements per source row)
        // blocklength=1
        // stride=ROWS (destination column stride in at[5][4])
        MPI_Type_vector(COLS, 1, ROWS, MPI_DOUBLE, &column_type);
        
        // transpose_type: repeat column_type ROWS (4) times, shifted by 1 element each
        // This handles all 4 source rows -> 4 destination columns
        // count=ROWS (4 source rows)
        // blocklength=1
        // stride=sizeof(double) (shift by 1 element for next column)
        MPI_Type_create_hvector(ROWS, 1, sizeof(double), column_type, &transpose_type);
        
        // Step 5: Commit the type
        MPI_Type_commit(&transpose_type);
        
        // Receive using the derived type
        // The received data will be placed in transposed order in at
        MPI_Recv(&at[0][0], 1, transpose_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Step 7: Display matrix at on process 1 after reception
        printf("Process 1 - Matrix transposee at:\n");
        for (int i = 0; i < COLS; i++) {
            for (int j = 0; j < ROWS; j++) {
                printf("%4.0f ", at[i][j]);
            }
            printf("\n");
        }
        
        // Free the derived types
        MPI_Type_free(&column_type);
        MPI_Type_free(&transpose_type);
    }
    
    MPI_Finalize();
    return 0;
}
