// #include <mpi.h>
// #include <stdio.h>

// int main(int argc, char **argv)
// {
//     MPI_Init(&argc, &argv);

//     int rank;
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);


//     int data = rank;
//     int received;

//     printf("Process %d: waiting to receive...\n", rank);
//     MPI_Recv(&received, 1, MPI_INT, 1-rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     printf("Process %d: Now sending %d ...\n", rank, data);
//     MPI_Send(&data, 1, MPI_INT, 1-rank, 0, MPI_COMM_WORLD);

//     printf("Process %d: Done\n", rank);

//     MPI_Finalize();
//     return 0;   
// }


#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int data = rank;
    int received;
    
    if (rank == 0) {
        // Process 0: Send first, then receive
        MPI_Send(&data, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&received, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {
        // Process 1: Receive first, then send
        MPI_Recv(&received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Send(&data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    printf("Process %d: Received %d\n", rank, received);
    
    MPI_Finalize();
    return 0;
}