#include <stdio.h>
#include <mpi.h>



int main(int argc, char *argv[]){
    int rank, size; 
    MPI_Init(&argc, &argv) ;
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    // qst 1:
    // printf("Hello World, from process %d\n",rank) ;
    // qst 2:
    // printf("I am process %d among %d\n", rank, size);
    //qst 3:
    if (rank==1)
        printf("I am process %d among %d\n", rank, size);
    
    MPI_Finalize();
    return  0 ;
}