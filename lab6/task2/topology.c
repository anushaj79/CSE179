#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    int rank, cart_rank, numprocs;
    int dims[2] = {4, 4};
    int periods[2] = {1, 1};
    int coords[2];
    int n, s, e, w;
    MPI_Comm cart_comm;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (numprocs != 16) {
        if (rank == 0) printf("Error: This task requires exactly 16 processes.\n");
        MPI_Finalize();
        return 0;
    }

    // Cartesian communicator 
    MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, 0, &cart_comm);
    MPI_Comm_rank(cart_comm, &cart_rank);
    MPI_Cart_coords(cart_comm, cart_rank, 2, coords);

    // compare ranks 
    printf("Global Rank: %d | Cart Rank: %d | Coords: (%d, %d)\n", 
            rank, cart_rank, coords[0], coords[1]);

    // find neighbors
    MPI_Cart_shift(cart_comm, 0, 1, &n, &s);
    MPI_Cart_shift(cart_comm, 1, 1, &w, &e);

    float my_val = (float)cart_rank;
    float n_val, s_val, e_val, w_val;

    MPI_Sendrecv(&my_val, 1, MPI_FLOAT, n, 0, &s_val, 1, MPI_FLOAT, s, 0, cart_comm, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&my_val, 1, MPI_FLOAT, s, 0, &n_val, 1, MPI_FLOAT, n, 0, cart_comm, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&my_val, 1, MPI_FLOAT, e, 0, &w_val, 1, MPI_FLOAT, w, 0, cart_comm, MPI_STATUS_IGNORE);
    MPI_Sendrecv(&my_val, 1, MPI_FLOAT, w, 0, &e_val, 1, MPI_FLOAT, e, 0, cart_comm, MPI_STATUS_IGNORE);

    float avg = (my_val + n_val + s_val + e_val + w_val) / 5.0;
    printf("Cart Rank %d Average with neighbors: %.2f\n", cart_rank, avg);

    MPI_Finalize();
    return 0;
}