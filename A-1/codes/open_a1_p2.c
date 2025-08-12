#include <stdio.h>
#include <omp.h> 

int main() {
    int num_threads;

    // taking user input for number of threads
    printf("Enter the number of threads to use: ");
    scanf("%d", &num_threads);

    printf("\n--- Sequential Execution ---\n");
    // using single thread for loop based
    for (int i = 0; i < num_threads; i++) {
        printf("Hello, World from sequential loop, iteration %d\n", i);
    }

    printf("\n--- Parallel Execution ---\n");
    // setting the number ff threads 
    omp_set_num_threads(num_threads);

    // initializing threads
    #pragma omp parallel
    {
        // fetch the thread id
        int thread_id = omp_get_thread_num();
        printf("Hello, World from thread %d\n", thread_id);
    }

    return 0;
}