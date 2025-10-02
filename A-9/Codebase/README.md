# HPC Practical No. 9 - Exploratory & Speculative Decomposition in Parallel Programming

## Overview

This practical implements two parallel programming decomposition strategies:

1. **Exploratory Decomposition**: N-Queens Problem
2. **Speculative Decomposition**: If-Else Branch Evaluation

## Files Structure

```
A-9/
├── exploratory_nqueens.c    # N-Queens with exploratory decomposition
├── speculative_branch.c     # Branch evaluation with speculative decomposition
├── run_experiments.sh       # Script to compile and run both programs
└── README.md               # This file
```

## Prerequisites

- GCC compiler with OpenMP support
- Multi-core CPU (recommended ≥2 cores)
- Linux/Unix environment

## How to Run

### Method 1: Using the automated script

```bash
chmod +x run_experiments.sh
./run_experiments.sh
```

### Method 2: Manual compilation and execution

```bash
# Compile programs
gcc -fopenmp -O2 -o exploratory_nqueens exploratory_nqueens.c -lm
gcc -fopenmp -O2 -o speculative_branch speculative_branch.c -lm

# Run programs
./exploratory_nqueens
./speculative_branch
```

## Program Descriptions

### 1. Exploratory Decomposition - N-Queens Problem

- **File**: `exploratory_nqueens.c`
- **Approach**: Distributes the initial queen placements among different threads
- **Key Feature**: Each thread explores a disjoint region of the solution space
- **Expected Behavior**:
  - No wasted computation (all work contributes to final result)
  - Good speedup on multi-core systems
  - Finds all valid solutions to the N-Queens problem

### 2. Speculative Decomposition - Branch Evaluation

- **File**: `speculative_branch.c`
- **Approach**: Computes both branches of an if-else statement in parallel
- **Key Feature**: One branch result is discarded based on condition evaluation
- **Expected Behavior**:
  - ~50% wasted computation (one branch is always discarded)
  - Moderate speedup due to parallel execution
  - Trade-off between latency reduction and computational overhead

## Expected Results

| Problem                         | Sequential Time (ms) | Parallel Time (ms) | Speedup  | Wasted Computation (%) |
| ------------------------------- | -------------------- | ------------------ | -------- | ---------------------- |
| N-Queens (Exploratory)          | ~800-1200            | ~300-600           | 2.0-3.0x | ~0%                    |
| Branch Evaluation (Speculative) | ~900-1500            | ~500-900           | 1.5-2.5x | ~50%                   |

_Note: Actual results depend on your CPU cores and system load_

## Key Learning Points

### Exploratory Decomposition

- **Advantages**: No wasted work, good scalability
- **Use Cases**: Search problems, backtracking algorithms
- **Challenges**: Load balancing, irregular workloads

### Speculative Decomposition

- **Advantages**: Reduced latency, parallelizes conditional code
- **Use Cases**: Branch-heavy code, uncertain conditions
- **Challenges**: Resource overhead, wasted computation

## Troubleshooting

### Compilation Issues

- Ensure GCC supports OpenMP: `gcc --version`
- Check OpenMP support: `echo | gcc -fopenmp -dM -E - | grep -i openmp`

### Runtime Issues

- If speedup is poor, check CPU cores: `nproc`
- Monitor CPU usage during execution: `htop` or `top`

### Performance Tuning

- Adjust problem size in the code if needed
- Set number of threads: `export OMP_NUM_THREADS=4`

## Code Structure

### N-Queens Implementation

```c
// Key functions:
- is_safe(): Check queen placement validity
- solve_nqueens_sequential(): Sequential backtracking
- solve_nqueens_parallel(): Parallel exploratory search
```

### Speculative Branch Implementation

```c
// Key functions:
- compute_branch1(): First branch computation (sqrt)
- compute_branch2(): Second branch computation (log)
- sequential_computation(): Traditional if-else
- speculative_computation(): Parallel branch execution
```

## Observations Template

When running the experiments, note:

1. **Speedup achieved**: Compare sequential vs parallel times
2. **Scalability**: How performance changes with available cores
3. **Overhead**: Impact of thread creation and synchronization
4. **Wasted work**: Quantify discarded computation in speculation
5. **Load balancing**: Distribution of work among threads

## Report Guidelines

Include in your technical report:

1. **Introduction**: Brief explanation of both decomposition techniques
2. **Implementation**: Algorithm design and key code snippets
3. **Results**: Timing tables and performance graphs
4. **Analysis**: Speedup calculations and efficiency metrics
5. **Conclusions**: Trade-offs and applicability of each approach

---

**Author**: HPC Lab Student  
**Date**: October 2025  
**Course**: High Performance Computing Laboratory
