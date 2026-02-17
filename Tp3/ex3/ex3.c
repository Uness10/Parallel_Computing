#include <stdio.h>
#include <omp.h>
static long num_steps = 100000;
double step;

int main() {
    int i; double x, pi, sum = 0.0;
    step = 1.0 / (double) num_steps;
    double time1 = omp_get_wtime(); 

    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
        double time2 = omp_get_wtime(); 
    printf("Time = %f\n",time2-time1);

    pi = step * sum;
    printf("pi = %f\n", pi);
    return 0;
}
