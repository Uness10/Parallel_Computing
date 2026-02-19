#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#define WORK_SIZE 1000000

void task_light(int N) {
    double x = 0.0;
    for (int i = 0; i < N; i++) {
        x += sin(i * 0.001);
    }
    printf("  Task A (light)    result = %f\n", x);
}

void task_moderate(int N) {
    double x = 0.0;
    for (int i = 0; i < 5*N; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001);
    }
    printf("  Task B (moderate) result = %f\n", x);
}

void task_heavy(int N) {
    double x = 0.0;
    for (int i = 0; i < 20*N; i++) {
        x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
    }
    printf("  Task C (heavy)    result = %f\n", x);
}

int main() {
    double start, end;

    /* =============================================
     * VERSION 1: Sequential
     * ============================================= */
    printf("=== Sequential Version ===\n");
    start = omp_get_wtime();

    task_light(WORK_SIZE);
    task_moderate(WORK_SIZE);
    task_heavy(WORK_SIZE);

    end = omp_get_wtime();
    printf("Sequential time = %lf seconds\n\n", end - start);

    /* =============================================
     * VERSION 2: Parallel Sections (basic)
     * ============================================= */
    printf("=== Parallel Sections Version ===\n");
    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            printf("Thread %d executing Task A (light)\n", omp_get_thread_num());
            task_light(WORK_SIZE);
        }

        #pragma omp section
        {
            printf("Thread %d executing Task B (moderate)\n", omp_get_thread_num());
            task_moderate(WORK_SIZE);
        }

        #pragma omp section
        {
            printf("Thread %d executing Task C (heavy)\n", omp_get_thread_num());
            task_heavy(WORK_SIZE);
        }
    }

    end = omp_get_wtime();
    printf("Parallel sections time = %lf seconds\n\n", end - start);

    /* =============================================
     * VERSION 3: Optimized - Split heavy task for better load balancing
     * ============================================= */
    printf("=== Optimized Parallel Sections (split heavy task) ===\n");
    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        /* Section 1: Light + Moderate combined (1N + 5N = 6N iterations) */
        #pragma omp section
        {
            printf("Thread %d executing Task A (light) + Task B (moderate)\n",
                   omp_get_thread_num());
            task_light(WORK_SIZE);
            task_moderate(WORK_SIZE);
        }

        /* Section 2: First half of heavy (10N iterations) */
        #pragma omp section
        {
            printf("Thread %d executing Task C first half (heavy)\n",
                   omp_get_thread_num());
            double x = 0.0;
            for (int i = 0; i < 10*WORK_SIZE; i++) {
                x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
            }
            printf("  Task C half-1 result = %f\n", x);
        }

        /* Section 3: Second half of heavy (10N iterations) */
        #pragma omp section
        {
            printf("Thread %d executing Task C second half (heavy)\n",
                   omp_get_thread_num());
            double x = 0.0;
            for (int i = 10*WORK_SIZE; i < 20*WORK_SIZE; i++) {
                x += sqrt(i * 0.5) * cos(i * 0.001) * sin(i * 0.0001);
            }
            printf("  Task C half-2 result = %f\n", x);
        }
    }

    end = omp_get_wtime();
    printf("Optimized parallel sections time = %lf seconds\n", end - start);

    return 0;
}
