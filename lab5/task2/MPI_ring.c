#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int numprocs, rank, message;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (numprocs < 2) {
        if (rank == 0) printf("Please run with at least 2 processes.\n");
        MPI_Finalize();
        return 0;
    }

    double start_time = MPI_Wtime();

    if (rank == 0) {
        // if rank 0 then start the ring
        // send to rank 1 and receive from last rank
        message = 0;
        printf("Process 0 starting ring with value: %d\n", message);
        
        MPI_Send(&message, 1, MPI_INT, 1, 0, MPI_COMM_WORLD); 
        MPI_Recv(&message, 1, MPI_INT, numprocs - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        printf("Process 0 received final value %d from Process %d\n", message, numprocs - 1);
    } 
    else {
        // for other ranks receive from prev rank and send to next rank
        MPI_Recv(&message, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        if (message == rank - 1) {
            message = rank; // update message to current rank
        }
        int dest = (rank + 1) % numprocs; 

        MPI_Send(&message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
    }

    double end_time = MPI_Wtime();
    
    if (rank == 0) {
        printf("Total execution time for %d processes: %f seconds\n", numprocs, end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}