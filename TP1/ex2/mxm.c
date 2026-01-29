#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define NUM_SIZES 10

int main(){
    int sizes[NUM_SIZES] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    double time_ijk[NUM_SIZES];
    double time_ikj[NUM_SIZES];
    double bandwidth_ijk[NUM_SIZES];
    double bandwidth_ikj[NUM_SIZES];
    
    for(int s = 0; s < NUM_SIZES; s++){
        int n = sizes[s];
        
        // Dynamically allocate matrices
        int **a = malloc(n * sizeof(int*));
        int **b = malloc(n * sizeof(int*));
        int **c = malloc(n * sizeof(int*));
        for(int i = 0; i < n; i++){
            a[i] = malloc(n * sizeof(int));
            b[i] = malloc(n * sizeof(int));
            c[i] = malloc(n * sizeof(int));
        }
        
        // Initialize matrices
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                a[i][j] = i + j;
                b[i][j] = i - j;
            }
        }
        
        clock_t start, end;
        long long memory_ops;
        
        // Version 1: i-j-k loop order
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                c[i][j] = 0;
        
        start = clock();
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        end = clock();
        time_ijk[s] = ((double)(end - start)) / CLOCKS_PER_SEC;
        memory_ops = 4LL * n * n * n * sizeof(int);
        bandwidth_ijk[s] = memory_ops / time_ijk[s] / (1024.0 * 1024.0 * 1024.0);
        
        // Version 2: i-k-j loop order
        for(int i = 0; i < n; i++)
            for(int j = 0; j < n; j++)
                c[i][j] = 0;
        
        start = clock();
        for(int i = 0; i < n; i++){
            for(int k = 0; k < n; k++){
                for(int j = 0; j < n; j++){
                    c[i][j] += a[i][k] * b[k][j];
                }
            }
        }
        end = clock();
        time_ikj[s] = ((double)(end - start)) / CLOCKS_PER_SEC;
        bandwidth_ikj[s] = memory_ops / time_ikj[s] / (1024.0 * 1024.0 * 1024.0);
        
        printf("n=%d: ijk=%.4fs (%.2f GB/s), ikj=%.4fs (%.2f GB/s)\n", 
               n, time_ijk[s], bandwidth_ijk[s], time_ikj[s], bandwidth_ikj[s]);
        
        // Free memory
        for(int i = 0; i < n; i++){
            free(a[i]);
            free(b[i]);
            free(c[i]);
        }
        free(a);
        free(b);
        free(c);
    }
    
    printf("\n--- CSV Data for Plotting ---\n");
    printf("n,time_ijk,time_ikj,bandwidth_ijk,bandwidth_ikj\n");
    for(int s = 0; s < NUM_SIZES; s++){
        printf("%d,%.6f,%.6f,%.2f,%.2f\n", 
               sizes[s], time_ijk[s], time_ikj[s], bandwidth_ijk[s], bandwidth_ikj[s]);
    }
    
    return 0;
}