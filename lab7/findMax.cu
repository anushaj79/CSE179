#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cuda_runtime.h>

#define BLOCK_SIZE 1024

__global__ void findMaxKernel(float *d_in, float *d_out, int n) {
    extern __shared__ float sdata[];

    unsigned int tid = threadIdx.x;
    unsigned int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        sdata[tid] = d_in[i];
    } 
    else {
        sdata[tid] = -1e38f; 
        }
    __syncthreads(); 

    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) {
            if (sdata[tid + s] > sdata[tid]) {
                sdata[tid] = sdata[tid + s];
            }
        }
        __syncthreads(); 
    }

    if (tid == 0) d_out[blockIdx.x] = sdata[0];
}

int main(int argc, char **argv) {
    // input vector size
    int n;
    std::cout << "Enter an integer: ";
    std::cin >> n;

    // size in bytes for mem allocation
    size_t size = n * sizeof(float);

    // rand vec V
    std::vector<float> h_in(n);
    std::srand(std::time(nullptr));
    for (int i = 0; i < n; i++) {
        h_in[i] = static_cast<float>(std::rand()) / RAND_MAX;
    }

    // cpu
    double cpu_start = std::clock();
    float cpu_max = -1e38f;
    for (int i = 0; i < n; i++) {
        if (h_in[i] > cpu_max) {
            cpu_max = h_in[i];
        }
    }
    double cpu_end = std::clock();
    double cpu_duration = 1000.0 * (cpu_end - cpu_start) / CLOCKS_PER_SEC;

    float *d_in, *d_out;
    int numBlocks = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;
    cudaMalloc(&d_in, size);
    cudaMalloc(&d_out, numBlocks * sizeof(float));
    cudaMemcpy(d_in, h_in.data(), size, cudaMemcpyHostToDevice);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start);

    findMaxKernel<<<numBlocks, BLOCK_SIZE, BLOCK_SIZE * sizeof(float)>>>(d_in, d_out, n);

    cudaEventRecord(stop);
    cudaEventSynchronize(stop);
    float gpu_milliseconds = 0;
    cudaEventElapsedTime(&gpu_milliseconds, start, stop);

    std::vector<float> h_out(numBlocks);
    cudaMemcpy(h_out.data(), d_out, numBlocks * sizeof(float), cudaMemcpyDeviceToHost);

    float gpu_max = -1e38f;
    for (int i = 0; i < h_out.size(); i++) {
        if (h_out[i] > gpu_max) {
            gpu_max = h_out[i];
    }
}

    std::cout << "Vector Size N: " << n << std::endl;
    std::cout << "CPU Max: " << cpu_max << " (Time: " << cpu_duration << " ms)" << std::endl;
    std::cout << "GPU Max: " << gpu_max << " (Time: " << gpu_milliseconds << " ms)" << std::endl;

    cudaFree(d_in); cudaFree(d_out);
    return 0;
}