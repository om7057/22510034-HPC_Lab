#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ROWS 500
#define COLS 500

int main() {
    static int A[ROWS][COLS];
    static int x[COLS], y[ROWS];
    double start, end;

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            A[i][j] = i + j;
        }
    }
    for (int j = 0; j < COLS; j++) {
        x[j] = j;
    }

    start = omp_get_wtime();

    #pragma omp parallel for schedule(static)
    for (int i = 0; i < ROWS; i++) {
        int sum = 0;
        for (int j = 0; j < COLS; j++) {
            sum += A[i][j] * x[j];
        }
        y[i] = sum;
    }

    end = omp_get_wtime();
    printf("Execution time (Matrix-Vector): %f seconds\n", end - start);

    return 0;
}
