// Pragma Critical
#include <stdio.h>
#include <omp.h>

int main() {
    int counter = 0;
    int n = 100000;

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        #pragma omp critical
        {
            counter++;
        }
    }

    printf("Final counter (critical): %d\n", counter);
    return 0;
}



// Pragma Atomic
#include <stdio.h>
#include <omp.h>

int main() {
    int counter = 0;
    int n = 100000;

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        #pragma omp atomic
        counter++;
    }

    printf("Final counter (atomic): %d\n", counter);
    return 0;
}



//pragma omp_lock_t
#include <stdio.h>
#include <omp.h>

int main() {
    int counter = 0;
    int n = 100000;
    omp_lock_t lock;

    omp_init_lock(&lock);

    #pragma omp parallel for
    for (int i = 0; i < n; i++) {
        omp_set_lock(&lock);
        counter++;
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);

    printf("Final counter (lock): %d\n", counter);
    return 0;
}
