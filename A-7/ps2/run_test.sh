#!/bin/bash
# Complete Matrix-Matrix Multiplication Performance Test

echo "=== Matrix-Matrix Multiplication MPI Performance Test ==="

# Compile the program
echo "Compiling..."
mpicc matrix_matrix_mpi.c -o matrix_matrix_mpi

# Clear previous results
rm -f matrix_matrix_performance.txt

# Test with different process counts
echo "Running performance tests..."
for np in 1 2 4 8; do
    echo "  Testing with $np processes..."
    mpirun -np $np ./matrix_matrix_mpi
done

echo "Running Python analysis..."
python3 matrix_matrix_analysis.py

echo "Performance analysis complete!"
echo "Check 'matrix_matrix_performance.png' for graphs."