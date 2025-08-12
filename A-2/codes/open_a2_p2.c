#include <stdio.h>
#include <omp.h>

int main() {
    long num_steps = 1000000000; 
    double step_width = 1.0 / (double)num_steps;
    double total_sum = 0.0;
    
    int thread_counts[] = {1, 2, 4, 8, 12};
    int num_tests = sizeof(thread_counts) / sizeof(thread_counts[0]);

    printf("Pi Calculation using Numerical Integration\n");
    printf("Number of Steps: %ld\n\n", num_steps);
    printf("------------------------------------------------------------\n");
    printf("Threads |    Time (s)    |      Speedup   |  Calculated Pi\n");
    printf("------------------------------------------------------------\n");

    double time_taken_serial = 0.0;

    for (int i = 0; i < num_tests; i++) {
        int num_threads = thread_counts[i];
        omp_set_num_threads(num_threads);
        total_sum = 0.0; 

        double start_time = omp_get_wtime();

        // 'reduction(+:total_sum)' clause creates a private copy of 'total_sum' for each thread.
        // avoiding race condition 
        #pragma omp parallel for reduction(+:total_sum)
        for (long j = 0; j < num_steps; j++) {
            double x = (j + 0.5) * step_width;
            total_sum += 4.0 / (1.0 + x * x);
        }

        double pi = step_width * total_sum;
        double end_time = omp_get_wtime();
        double time_taken = end_time - start_time;
        
        if (i == 0) {
            time_taken_serial = time_taken;
        }

        double speedup = time_taken_serial / time_taken;
        
        printf("%5d   |    %10.6f  |    %8.4f    |  %.15f\n", num_threads, time_taken, speedup, pi);
    }

    
    return 0;
}