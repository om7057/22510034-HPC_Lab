#include <stdio.h>
#include <stdlib.h>
#include <omp.h> 

void vector_scalar_add(long size, double* vector, double scalar, double* result_vector) {
    // work sharing construct to parallelize addition of scalar
    #pragma omp parallel for
    for (long i = 0; i < size; i++) {
        result_vector[i] = vector[i] + scalar;
    }
}

int main() {
    // taking very large data size
    long data_size = 100000000; 
    double scalar = 3.14;
    int thread_counts[] = {1, 2, 4, 8, 12}; 
    int num_tests = sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("Vector-Scalar Addition\n");
    printf("Data Size: %ld elements\n", data_size);
    printf("Scalar Value: %f\n\n", scalar);
    printf("---------------------------------------------------\n");
    printf("Threads |    Time (s)    |      Speedup       \n");
    printf("---------------------------------------------------\n");

    // dynamic memory allocation
    double* vector = (double*)malloc(data_size * sizeof(double));
    double* result_vector = (double*)malloc(data_size * sizeof(double));

    if (vector == NULL || result_vector == NULL) {
        fprintf(stderr, "Memory allocation failed!\n");
        return 1;
    }

    
    for (long i = 0; i < data_size; i++) {
        vector[i] = (double)i;
    }

    double time_taken_serial = 0.0;

    for (int i = 0; i < num_tests; i++) {
        int num_threads = thread_counts[i];
        omp_set_num_threads(num_threads);

        double start_time = omp_get_wtime();
        vector_scalar_add(data_size, vector, scalar, result_vector);
        double end_time = omp_get_wtime();

        double time_taken = end_time - start_time;

        if (i == 0) {
            time_taken_serial = time_taken;
        }

        double speedup = time_taken_serial / time_taken;

        printf("%5d   |    %10.6f  |      %8.4f      \n", num_threads, time_taken, speedup);
    }
  

    
    free(vector);
    free(result_vector);

    return 0;
}