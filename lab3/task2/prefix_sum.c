#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 4096
#define THREADS 4

int main() {
    int x[N], y[N], T[THREADS];
    double start, start_step_time, step_times[4];

    for (int i = 0; i < N; i++) x[i] = rand() % 10; //filling input arr w/ rand ints

    start = omp_get_wtime();

    // 1) split X among threads and compute each partial prefix sum in parallel
    start_step_time = omp_get_wtime();
    #pragma omp parallel num_threads(THREADS)
    {
        int tid = omp_get_thread_num();
        int chunk = N / THREADS;
        int low = tid * chunk;
        int high = (tid + 1) * chunk;

        y[low] = x[low];
        for (int i = low + 1; i < high; i++) {
            y[i] = y[i - 1] + x[i];
        }
        T[tid] = y[high - 1];
    }
    step_times[0] = omp_get_wtime() - start_step_time;

    // 2) perform prefix sum on last sum of each thread but the final thread in sequential order
    start_step_time = omp_get_wtime();
    int temp[THREADS];
    temp[0] = 0; // First thread has no offset
    for (int i = 1; i < THREADS; i++) {
        temp[i] = temp[i - 1] + T[i - 1];
    }
    step_times[1] = omp_get_wtime() - start_step_time;

    // 3) add offset to each thread's partial sums in parallel
    start_step_time = omp_get_wtime();
    #pragma omp parallel num_threads(THREADS)
    {
        int tid = omp_get_thread_num();
        int chunk = N / THREADS;
        int low = tid * chunk;
        int high = (tid + 1) * chunk;

        for (int i = low; i < high; i++) {
            y[i] += temp[tid];
        }
    }
    step_times[2] = omp_get_wtime() - start_step_time;

    step_times[3] = 0; 

    double final_time = omp_get_wtime() - start;

    printf("Total Execution Time: %f seconds\n", final_time);
    for(int i = 0; i < 3; i++) {
        printf("Step %d Execution Time: %f seconds\n", i+1, step_times[i]);
    }
    
    printf("Final Sum Check: %d\n", y[N-1]);

    return 0;
}