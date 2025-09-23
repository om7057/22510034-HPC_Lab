#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8}; 
    int sum = 0;
    
    if (rank == 0) {
        // P0: Sum first half (0,1,2,3)
        for (int i = 0; i < 4; i++) {
            sum += array[i];
        }
        printf("P0 sum = %d\n", sum);
        
        // Get P1's sum
        int p1_sum;
        MPI_Recv(&p1_sum, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        printf("Total sum = %d\n", sum + p1_sum);
        
    } else {
        // P1: Sum second half (4,5,6,7)
        for (int i = 4; i < 8; i++) {
            sum += array[i];
        }
        printf("P1 sum = %d\n", sum);
        
        // Send to P0
        MPI_Send(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    MPI_Finalize();
    return 0;
}