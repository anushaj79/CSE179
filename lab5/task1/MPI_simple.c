#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int numprocs, rank, namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv); // init mpi env
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs); //get total num processes
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get current rank
    MPI_Get_processor_name(processor_name, &namelen); //get machine name
    
    printf("Process %d on %s out of %d\n", rank, processor_name, numprocs);
    MPI_Finalize(); //mpi terminate
}