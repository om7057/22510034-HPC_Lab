#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int N = 1000;
    double *matrix = (double*)malloc(N * N * sizeof(double));
    double *vector = (double*)malloc(N * sizeof(double));
    double *result = (double*)calloc(N, sizeof(double));
    
    
    if (rank == 0) {
        for (int i = 0; i < N * N; i++) {
            matrix[i] = (double)(i % 10 + 1);
        }
        for (int i = 0; i < N; i++) {
            vector[i] = (double)(i % 10 + 1);
        }
    }
    
    MPI_Bcast(vector, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    int rows_per_process = N / size;
    int start_row = rank * rows_per_process;
    int end_row = (rank == size - 1) ? N : start_row + rows_per_process;
    
    double *local_matrix = (double*)malloc((end_row - start_row) * N * sizeof(double));
    
    if (rank == 0) {
        for (int p = 1; p < size; p++) {
            int p_start = p * rows_per_process;
            int p_end = (p == size - 1) ? N : p_start + rows_per_process;
            int p_rows = p_end - p_start;
            MPI_Send(&matrix[p_start * N], p_rows * N, MPI_DOUBLE, p, 0, MPI_COMM_WORLD);
        }
        for (int i = 0; i < (end_row - start_row) * N; i++) {
            local_matrix[i] = matrix[i];
        }
    } else {
        MPI_Recv(local_matrix, (end_row - start_row) * N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    double start_time = MPI_Wtime();
    
    double *local_result = (double*)calloc(end_row - start_row, sizeof(double));
    for (int i = 0; i < end_row - start_row; i++) {
        for (int j = 0; j < N; j++) {
            local_result[i] += local_matrix[i * N + j] * vector[j];
        }
    }
    
    double end_time = MPI_Wtime();
    
    // Gather results
    if (rank == 0) {
        for (int i = 0; i < end_row - start_row; i++) {
            result[start_row + i] = local_result[i];
        }
        for (int p = 1; p < size; p++) {
            int p_start = p * rows_per_process;
            int p_end = (p == size - 1) ? N : p_start + rows_per_process;
            int p_rows = p_end - p_start;
            MPI_Recv(&result[p_start], p_rows, MPI_DOUBLE, p, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        MPI_Send(local_result, end_row - start_row, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
    }
    
    if (rank == 0) {
        printf("Matrix-Vector Multiplication Results:\n");
        printf("Matrix size: %dx%d\n", N, N);
        printf("Number of processes: %d\n", size);
        printf("Time: %.6f seconds\n", end_time - start_time);
        
        printf("First 5 results: ");
        for (int i = 0; i < 5; i++) {
            printf("%.1f ", result[i]);
        }
        printf("\n\n");
        
        FILE *fp = fopen("simple_performance.txt", "a");
        fprintf(fp, "%d %.6f\n", size, end_time - start_time);
        fclose(fp);
    }
    
    free(matrix);
    free(vector);
    free(result);
    free(local_matrix);
    free(local_result);
    
    MPI_Finalize();
    return 0;
}