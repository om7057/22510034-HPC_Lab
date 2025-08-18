#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define BUFFER_SIZE 5
#define PRODUCE_COUNT 10

int buffer[BUFFER_SIZE];
int count = 0; 

int main() {
    double start, end;
    start = omp_get_wtime();

    #pragma omp parallel sections
    {
        // Producer Section
        #pragma omp section
        {
            for (int i = 1; i <= PRODUCE_COUNT; i++) {
                int produced_item = i;
                int placed = 0;

                while (!placed) {
                    #pragma omp critical
                    {
                        if (count < BUFFER_SIZE) {
                            buffer[count] = produced_item;
                            count++;
                            printf("Producer (Thread %d) produced item %d (Buffer count: %d)\n",
                                   omp_get_thread_num(), produced_item, count);
                            placed = 1;
                        }
                    }
                }
            }
        }

        // Consumer Section
        #pragma omp section
        {
            for (int i = 1; i <= PRODUCE_COUNT; i++) {
                int consumed_item = 0;
                int taken = 0;

                while (!taken) {
                    #pragma omp critical
                    {
                        if (count > 0) {
                            consumed_item = buffer[count - 1];
                            count--;
                            printf("Consumer (Thread %d) consumed item %d (Buffer count: %d)\n",
                                   omp_get_thread_num(), consumed_item, count);
                            taken = 1;
                        }
                    }
                }
            }
        }
    }

    end = omp_get_wtime();
    printf("Execution Time: %f seconds\n", end - start);

    return 0;
}
