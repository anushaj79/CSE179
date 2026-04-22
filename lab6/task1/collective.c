#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    int rank, numprocs;
    int root_val = 5;
    int chunk_size = 2; 

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    // task 1.1: broadcast  
    if (rank == 0) {
        root_val = root_val * root_val; 
    }
    
    MPI_Barrier(MPI_COMM_WORLD); 
    double start_1_1 = MPI_Wtime();
    
    MPI_Bcast(&root_val, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    double end_1_1 = MPI_Wtime();

    // task 1.2: scatter/gather 
    int *full_array = NULL;
    int *sub_array = (int *)malloc(chunk_size * sizeof(int));

    if (rank == 0) {
        full_array = (int *)malloc(numprocs * chunk_size * sizeof(int));
        for (int i = 0; i < numprocs * chunk_size; i++) full_array[i] = i;
    }

    MPI_Barrier(MPI_COMM_WORLD); 
    double start_1_2 = MPI_Wtime();

    MPI_Scatter(full_array, chunk_size, MPI_INT, sub_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < chunk_size; i++) sub_array[i] += rank;
    MPI_Gather(sub_array, chunk_size, MPI_INT, full_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    double end_1_2 = MPI_Wtime();

    if (rank == 0) {
        printf("Results for %d Processes \n", numprocs);
        printf("1.1 Broadcast Time:      %f seconds\n", end_1_1 - start_1_1);
        printf("1.2 Scatter/Gather Time: %f seconds\n", end_1_2 - start_1_2);
        free(full_array);
    }

    free(sub_array);
    MPI_Finalize();
    return 0;
}