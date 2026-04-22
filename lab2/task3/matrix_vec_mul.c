#include <iostream>
#include <vector>
#include "omp.h"

int main() {
    const int N = 10000; 
    std::vector<std::vector<double>> A(N, std::vector<double>(N));
    std::vector<double> x(N);
    std::vector<double> b(N, 0.0);
    
    omp_set_num_threads(4);
    double start_time = omp_get_wtime();

    #pragma omp parallel for schedule(guided)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            b[i] += A[i][j] * x[j];
        }
    }
    const double end_time = omp_get_wtime();

    std::cout << "Number of threads: " << omp_get_max_threads() << " Time taken: " << end_time - start_time << " seconds" << std::endl;

    return 0;
}