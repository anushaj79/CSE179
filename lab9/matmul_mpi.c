#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void generate_matrix(int *matrix, int n) {
    for (int i = 0; i < n * n; i++) {
        matrix[i] = rand() % 10;
    }
}

void serial_matmul(int *A, int *B, int *C, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            C[i * n + j] = 0;
            for (int k = 0; k < n; k++) {
                C[i * n + j] += A[i * n + k] * B[k * n + j];
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc < 3) {
        if (rank == 0) {
            printf("Usage: %s <n> <q>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int n = atoi(argv[1]);
    int q = atoi(argv[2]);

    // check process count
    if (size != q * q) {
        if (rank == 0) {
            printf("Error: Number of processes (%d) must equal q^2 (%d * %d)\n", size, q, q);
        }
        MPI_Finalize();
        return 1;
    }

    size_t matrix_size = n * n * sizeof(int);
    int *A = NULL;
    int *B = NULL;
    int *C = NULL;
    int *C_serial = NULL;

    if (rank == 0) {
        A = (int*)malloc(matrix_size);
        B = (int*)malloc(matrix_size);
        C = (int*)malloc(matrix_size);
        C_serial = (int*)malloc(matrix_size);

        srand(time(NULL));
        generate_matrix(A, n);
        generate_matrix(B, n);

        // Run serial multiplication for verification
        serial_matmul(A, B, C_serial, n);
    }

    int block_dim = n / q;
    size_t block_size = block_dim * block_dim * sizeof(int);

    int *A_block = (int*)malloc(block_size);
    int *B_block = (int*)malloc(block_size);
    int *C_block = (int*)calloc(block_dim * block_dim, sizeof(int));

    // broadcast A and B matrices to all processes
    if (rank != 0) {
        A = (int*)malloc(matrix_size);
        B = (int*)malloc(matrix_size);
    }
    
    MPI_Bcast(A, n * n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    // compute submatrix multiplication
    int block_row = (rank / q) * block_dim;
    int block_col = (rank % q) * block_dim;

    for (int i = 0; i < block_dim; i++) {
        for (int j = 0; j < block_dim; j++) {
            int sum = 0;
            for (int k = 0; k < n; k++) {
                sum += A[(block_row + i) * n + k] * B[k * n + (block_col + j)];
            }
            C_block[i * block_dim + j] = sum;
        }
    }

    int *recvbuf = NULL;
    if (rank == 0) {
        recvbuf = (int*)malloc(matrix_size);
    }

    MPI_Gather(C_block, block_dim * block_dim, MPI_INT,
               recvbuf, block_dim * block_dim, MPI_INT,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        // reconstruct matrix C
        for (int p = 0; p < size; p++) {
            int p_row = (p / q) * block_dim;
            int p_col = (p % q) * block_dim;
            for (int i = 0; i < block_dim; i++) {
                for (int j = 0; j < block_dim; j++) {
                    C[(p_row + i) * n + (p_col + j)] =
                        recvbuf[p * (block_dim * block_dim) + (i * block_dim + j)];
                }
            }
        }

        // verify computation against serial result
        int match = 1;
        for (int i = 0; i < n * n; i++) {
            if (C[i] != C_serial[i]) {
                match = 0;
                break;
            }
        }

        if (match) {
            printf("Verification Successful: Output matches the serial version.\n");
        } else {
            printf("Error: Mismatch in output verification.\n");
        }

        free(A);
        free(B);
        free(C);
        free(C_serial);
        free(recvbuf);
    }

    if (rank != 0) {
        free(A);
        free(B);
    }

    free(A_block);
    free(B_block);
    free(C_block);

    MPI_Finalize();
    return 0;
}