#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ROWS 500
#define COLS 500

int main() {
    static int A[ROWS][COLS];
    int scalar = 5;
    double start, end;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            A[i][j] = i + j;
        }
    }

    start = omp_get_wtime();

    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            A[i][j] = A[i][j] * scalar;
        }
    }

    end = omp_get_wtime();
    printf("Execution time (Matrix-Scalar): %f seconds\n", end - start);

    return 0;
}
