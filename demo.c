#include <stdio.h>
#include <omp.h> 

int main() {
    int n;

   
    printf("Enter the number of threads ");
    scanf("%d", &n);

    for (int i = 0; i < n; i++) {
        printf("Hello, World from sequential loop, iteration %d\n", i);
    }
    omp_set_num_threads(n);

    
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        printf("Thread %d\n", thread_id);
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        printf("iteration3 %d executed by thread %d\n", 
               i, omp_get_thread_num());
    }

    return 0;
}
