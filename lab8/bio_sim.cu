#include <iostream>
#include <vector>
#include <string>
#include <cuda_runtime.h>

__global__ void evolveKernel(int *current, int *next, int M, int N) {
    int r = blockIdx.y * blockDim.y + threadIdx.y;
    int c = blockIdx.x * blockDim.x + threadIdx.x;

    if (r < M && c < N) {
        int liveNeighbors = 0;

        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr;
                int nc = c + dc;

                if (nr >= 0 && nr < M && nc >= 0 && nc < N) {
                    liveNeighbors += current[nr * N + nc];
                }
            }
        }

        int currentState = current[r * N + c];
        int nextState = currentState;

        if (currentState == 1) {
            if (liveNeighbors <= 1 || liveNeighbors >= 4) {
                nextState = 0;
            }
        } else {
            if (liveNeighbors == 2 || liveNeighbors == 3) {
                nextState = 1;
            }
        }
        next[r * N + c] = nextState;
    }
}

void printMatrix(int *matrix, int M, int N) {
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << matrix[i * N + j];
        }
        std::cout << std::endl;
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " M N K [debug]" << std::endl;
        return 1;
    }

    int M = std::stoi(argv[1]);
    int N = std::stoi(argv[2]);
    int K = std::stoi(argv[3]);
    bool debug = (argc > 4);

    size_t size = M * N * sizeof(int);
    int *h_matrix = new int[M * N];

    for (int i = 0; i < M * N; i++) h_matrix[i] = rand() % 2;

    if (debug) {
        std::cout << "Start:" << std::endl;
        printMatrix(h_matrix, M, N);
    }

    int *d_current, *d_next;
    cudaMalloc(&d_current, size);
    cudaMalloc(&d_next, size);
    cudaMemcpy(d_current, h_matrix, size, cudaMemcpyHostToDevice);

    dim3 blockSize(16, 16);
    dim3 gridSize((N + blockSize.x - 1) / blockSize.x, (M + blockSize.y - 1) / blockSize.y);

    for (int k = 0; k < K; k++) {
        evolveKernel<<<gridSize, blockSize>>>(d_current, d_next, M, N);
        
        int *temp = d_current;
        d_current = d_next;
        d_next = temp;

        if (debug) {
            cudaMemcpy(h_matrix, d_current, size, cudaMemcpyDeviceToHost);
            std::cout << "Round " << k << ":" << std::endl;
            printMatrix(h_matrix, M, N);
        }
    }

    if (!debug) {
        cudaMemcpy(h_matrix, d_current, size, cudaMemcpyDeviceToHost);
        printMatrix(h_matrix, M, N);
    }

    cudaFree(d_current); cudaFree(d_next);
    delete[] h_matrix;
    return 0;
}