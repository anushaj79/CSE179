#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    int rank, numprocs;
    int data[10];
    int read_data[10];
    MPI_File fh;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

    for (int i = 0; i < 10; i++) data[i] = rank * 10 + i;

    MPI_File_open(MPI_COMM_WORLD, "test.bin", MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_Offset offset = rank * 10 * sizeof(int);
    MPI_File_seek(fh, offset, MPI_SEEK_SET);
    MPI_File_write(fh, data, 10, MPI_INT, &status);
    MPI_File_close(&fh);

    MPI_File_open(MPI_COMM_WORLD, "test.bin", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
    MPI_File_read_at(fh, offset, read_data, 10, MPI_INT, &status);
    
    int error_found = 0;
    for (int i = 0; i < 10; i++) if (read_data[i] != data[i]) error_found++;
    if (error_found == 0) printf("Rank %d: Read verification successful!\n", rank);
    MPI_File_close(&fh);

    MPI_Finalize();
    return 0;
}