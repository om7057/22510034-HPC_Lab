#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int N = 500; // Matrix size N x N
    double *A = (double *)malloc(N * N * sizeof(double));
    double *B = (double *)malloc(N * N * sizeof(double));
    double *C = (double *)calloc(N * N, sizeof(double));

    // Initialize matrices (only process 0)
    if (rank == 0)
    {
        for (int i = 0; i < N * N; i++)
        {
            A[i] = (double)(i % 10 + 1); // Values 1-10
            B[i] = (double)(i % 10 + 1); // Values 1-10
        }
        printf("Matrix-Matrix Multiplication: %dx%d matrices\n", N, N);
        printf("Number of processes: %d\n", size);
    }

    // Broadcast matrix B to all processes
    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Calculate rows per process
    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? N : start_row + rows_per_process;
    int my_rows = end_row - start_row;

    // Allocate local matrices
    double *local_A = (double *)malloc(my_rows * N * sizeof(double));
    double *local_C = (double *)calloc(my_rows * N, sizeof(double));

    // Distribute rows of matrix A
    if (rank == 0)
    {
        // Copy own rows
        for (int i = 0; i < my_rows * N; i++)
        {
            local_A[i] = A[i];
        }
        // Send rows to other processes
        for (int p = 1; p < size; p++)
        {
            int p_start = p * rows_per_process;
            int p_end = (p == size - 1) ? N : p_start + rows_per_process;
            int p_rows = p_end - p_start;
            MPI_Send(&A[p_start * N], p_rows * N, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
    }
    else
    {
        MPI_Recv(local_A, my_rows * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Start timing
    double start_time = MPI_Wtime();

    // Matrix multiplication: local_C = local_A * B
    for (int i = 0; i < my_rows; i++)
    {
        for (int j = 0; j < N; j++)
        {
            for (int k = 0; k < N; k++)
            {
                local_C[i * N + j] += local_A[i * N + k] * B[k * N + j];
            }
        }
    }

    // End timing
    double end_time = MPI_Wtime();

    // Gather results back to process 0
    if (rank == 0)
    {
        // Copy own results
        for (int i = 0; i < my_rows * N; i++)
        {
            C[i] = local_C[i];
        }
        // Receive from other processes
        for (int p = 1; p < size; p++)
        {
            int p_start = p * rows_per_process;
            int p_end = (p == size - 1) ? N : p_start + rows_per_process;
            int p_rows = p_end - p_start;
            MPI_Recv(&C[p_start * N], p_rows * N, MPI_DOUBLE, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }
    else
    {
        MPI_Send(local_C, my_rows * N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        printf("Execution time: %.6f seconds\n", end_time - start_time);

        // Show first few results for verification
        printf("First 3x3 of result matrix C:\n");
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                printf("%.0f ", C[i * N + j]);
            }
            printf("\n");
        }
        printf("\n");

        // Save performance data
        FILE *fp = fopen("matrix_matrix_performance.txt", "a");
        fprintf(fp, "%d %.6f\n", size, end_time - start_time);
        fclose(fp);
    }

    free(A);
    free(B);
    free(C);
    free(local_A);
    free(local_C);

    MPI_Finalize();
    return 0;
}