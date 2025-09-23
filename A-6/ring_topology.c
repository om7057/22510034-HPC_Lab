#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Ring topology: each process sends to next, receives from previous
    int next = (rank + 1) % size;        
    int prev = (rank - 1 + size) % size;

    int my_data = rank * 10;  
    int received_data;            
    printf("Process %d: sending %d to Process %d\n", rank, my_data, next);
    
    // even ranks send first, odd ranks receive first
    if (rank % 2 == 0) {
        MPI_Send(&my_data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
        MPI_Recv(&received_data, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        MPI_Recv(&received_data, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&my_data, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }
    
    printf("Process %d: received %d from Process %d\n", rank, received_data, prev);
    
    MPI_Finalize();
    return 0;
}