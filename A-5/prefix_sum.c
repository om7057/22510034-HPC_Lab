#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000

int main() {
    static int arr[N], prefix[N];
    double start, end;

    
    for (int i = 0; i < N; i++) {
        arr[i] = 1; 
    }

    start = omp_get_wtime();

    int num_threads;
    #pragma omp parallel
    {
        int id = omp_get_thread_num();
        int i, chunk_size;
        int local_sum = 0;

        #pragma omp single
        num_threads = omp_get_num_threads();

        chunk_size = N / num_threads;
        int start_idx = id * chunk_size;
        int end_idx = (id == num_threads - 1) ? N : start_idx + chunk_size;

        // Step 1: Local prefix sum in each chunk
        for (i = start_idx; i < end_idx; i++) {
            local_sum += arr[i];
            prefix[i] = local_sum;
        }

        // Step 2: Store total sum of each chunk's last element
        static int chunk_sums[100]; 
        chunk_sums[id] = local_sum;

        #pragma omp barrier

        // Step 3: Accumulate sums from previous chunks
        int offset = 0;
        for (i = 0; i < id; i++) {
            offset += chunk_sums[i];
        }

        // Step 4: Add offset to local chunk
        for (i = start_idx; i < end_idx; i++) {
            prefix[i] += offset;
        }
    }

    end = omp_get_wtime();

    printf("Execution time (Prefix Sum): %f seconds\n", end - start);
    printf("First 10 prefix sums: ");
    for (int i = 0; i < 10; i++) {
        printf("%d ", prefix[i]);
    }
    printf("\n");

    return 0;
}
