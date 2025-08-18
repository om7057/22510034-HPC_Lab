#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 20   

int main() {
    int fib[N];
    fib[0] = 0;
    fib[1] = 1;

    double start, end;
    start = omp_get_wtime();

    #pragma omp parallel
    {
        // Step 1: Only one thread initializes first two terms
        #pragma omp single
        {
            printf("Thread %d initialized first two terms\n", omp_get_thread_num());
        }

        // Step 2: Parallel computation of Fibonacci sequence
        #pragma omp for schedule(static) nowait
        for (int i = 2; i < N; i++) {
            
            // Protect shared updates
            #pragma omp critical
            {
                fib[i] = fib[i - 1] + fib[i - 2];
            }
            printf("Thread %d computed fib[%d] = %d\n", omp_get_thread_num(), i, fib[i]);
        }

        // Step 3: Barrier before printing
        #pragma omp barrier

        // Step 4: Safe printing
        #pragma omp for schedule(static)
        for (int i = 0; i < N; i++) {
            #pragma omp critical
            {
                printf("fib[%d] = %d\n", i, fib[i]);
            }
        }
    }

    end = omp_get_wtime();
    printf("Execution Time: %f seconds\n", end - start);

    return 0;
}
