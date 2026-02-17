#include <stdio.h>
#include <omp.h>

int main(){
    int rank ;
    int mx = 0 ; 
    #pragma omp parallel reduction(max:mx)
    {
        rank = omp_get_thread_num() ; 
        printf("Hello from the rank %d thread\n",rank);
        mx = rank;
    }  
    printf("Parallel execution of hello_world with %d threads\n",mx+1);
    return 0 ;
}