#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 

#define SAMPLES_PER_THREAD 10000000

void *compute_pi(void *arg) {
    int seed = *(int *)arg;
    int local_hits = 0;
    double x, y;

    for (int i = 0; i < SAMPLES_PER_THREAD; i++) {
        x = (double)rand_r(&seed) / RAND_MAX - 0.5;
        y = (double)rand_r(&seed) / RAND_MAX - 0.5;

        if ((x * x + y * y) < 0.25) { // x^2 + y^2 < r^2 
            local_hits++;
        }
    }

    *(int *)arg = local_hits;
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    int hits[num_threads];
    int total_hits = 0;

    double start_time = omp_get_wtime(); 

    for (int i = 0; i < num_threads; i++) {
        hits[i] = i + 1; 
        pthread_create(&threads[i], NULL, compute_pi, (void *)&hits[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        total_hits += hits[i];
    }

    double end_time = omp_get_wtime();

    // pi = 4 * (total_hits) / (total_samples)
    double pi_est = 4.0 * total_hits / (num_threads * SAMPLES_PER_THREAD);

    printf("Threads: %d\n", num_threads);
    printf("Estimated PI: %f\n", pi_est);
    printf("Execution Time: %f seconds\n", end_time - start_time);

    return 0;
}