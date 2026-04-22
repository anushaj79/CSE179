#include <iostream>
#include "omp.h"

int main() {
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int total = omp_get_num_threads();

        #pragma omp critical
        std::cout << "Hello World! Thread ID: " << id << ", Total Threads: " << total << std::endl;
    }
        return 0;
}