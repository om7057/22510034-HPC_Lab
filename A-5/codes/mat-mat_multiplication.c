#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500 

int main() {
    static int A[N][N], B[N][N], C[N][N];
    double start, end;

    // Initialize matrices
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
            C[i][j] = 0;
        }
    }

    start = omp_get_wtime();

    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    end = omp_get_wtime();
    printf("Execution time (Matrix-Matrix): %f seconds\n", end - start);

    return 0;
}
