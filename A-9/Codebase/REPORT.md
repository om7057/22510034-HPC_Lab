# HPC Practical No. 9 - Technical Report

## Exploratory & Speculative Decomposition in Parallel Programming

**Student Name:** [Your Name]  
**Roll Number:** [Your Roll Number]  
**Date:** October 2025

---

## 1. Introduction

This practical implements and compares two parallel programming decomposition strategies:

### 1.1 Exploratory Decomposition

Exploratory decomposition partitions a search or solution space into disjoint regions that can be explored concurrently. Each thread works on a separate portion of the problem space, ensuring that no computational work is wasted. This approach is particularly effective for problems like:

- Backtracking algorithms (N-Queens)
- Tree traversal
- Search problems with large solution spaces

### 1.2 Speculative Decomposition

Speculative decomposition executes multiple possible computation paths in parallel before the controlling condition is resolved. Once the condition is known, the correct result is selected and the rest are discarded. This technique:

- Reduces latency by computing results in advance
- Trades computational resources for reduced execution time
- Involves inherent waste due to discarded computations

---

## 2. Problem Descriptions

### 2.1 Part A: N-Queens Problem (Exploratory Decomposition)

**Problem:** Place N queens on an N×N chessboard such that no two queens attack each other.

**Sequential Approach:**

- Backtracking algorithm exploring all possible placements
- Depth-first search with constraint checking

**Parallel Approach:**

- Distribute initial queen placements among threads
- Each thread explores a disjoint portion of the solution tree
- No communication needed between threads during exploration

### 2.2 Part B: If-Else Branch Evaluation (Speculative Decomposition)

**Problem:** Evaluate conditional branches where the condition evaluation might be expensive or the branches have significant computational cost.

**Sequential Approach:**

- Evaluate condition first
- Execute only the required branch (sqrt(x) if x > 0, else log(|x|))

**Parallel Approach:**

- Execute both branches simultaneously using parallel sections
- Select the correct result based on the condition after both complete
- Discard the unused computation

---

## 3. Algorithm Design

### 3.1 N-Queens Implementation

```
Algorithm: Parallel N-Queens
Input: Board size N
Output: Total number of solutions

1. Sequential function solve_nqueens_sequential(board, row, count):
   - If row == N: increment count, return
   - For each column in row:
     - If placement is safe:
       - Place queen, recurse to next row

2. Parallel function solve_nqueens_parallel(board, row, count):
   - If row == 0 (first level):
     - #pragma omp parallel for
     - Distribute columns among threads
     - Each thread gets local copy of board
     - Call sequential solver for remaining rows
   - Else: continue sequential solving
```

### 3.2 Speculative Branch Implementation

```
Algorithm: Speculative Branch Evaluation
Input: Value x, number of iterations
Output: Computed results

1. Sequential approach:
   - If x > 0: compute sqrt branch
   - Else: compute log branch

2. Speculative approach:
   - #pragma omp parallel sections num_threads(2)
   - Section 1: compute sqrt branch
   - Section 2: compute log branch
   - Select correct result based on condition
   - Discard unused result
```

---

## 4. Implementation Details

### 4.1 System Configuration

- **CPU Cores:** 12
- **Compiler:** GCC with OpenMP support
- **Optimization:** -O2 flag enabled
- **Problem Sizes:** N-Queens (12×12), Branch Evaluation (100,000 iterations)

### 4.2 Key Code Snippets

**N-Queens Parallel Distribution:**

```c
#pragma omp parallel for
for (int col = 0; col < n; col++) {
    int local_board[n];
    memcpy(local_board, board, sizeof(int) * n);
    // Process each starting column in parallel
}
```

**Speculative Parallel Sections:**

```c
#pragma omp parallel sections num_threads(2)
{
    #pragma omp section
    { result1 = compute_branch1(x); }
    #pragma omp section
    { result2 = compute_branch2(x); }
}
```

### 4.3 Synchronization and Thread Safety

- **N-Queens:** Atomic operations for result counting
- **Speculative:** Thread-safe counters for tracking computations
- **Memory Management:** Local copies to avoid race conditions

---

## 5. Results

### 5.1 Performance Measurements

| Problem                         | Sequential Time (ms) | Parallel Time (ms) | Speedup | Wasted Computation (%) |
| ------------------------------- | -------------------- | ------------------ | ------- | ---------------------- |
| N-Queens (Exploratory)          | 91.57                | 17.79              | 5.15×   | ~0%                    |
| Branch Evaluation (Speculative) | 136.23               | 247.07             | 0.55×   | ~50%                   |

### 5.2 Detailed Statistics

**N-Queens Problem:**

- Sequential: Avg 91.57ms (Min: 90.13ms, Max: 92.90ms)
- Parallel: Avg 17.79ms (Min: 12.87ms, Max: 22.77ms)
- Efficiency: 43% (5.15 speedup on 12 cores)
- Solutions found: 14,200 (verified correct)

**Branch Evaluation:**

- Sequential: Avg 136.23ms (Min: 132.99ms, Max: 140.84ms)
- Parallel: Avg 247.07ms (Min: 241.47ms, Max: 263.99ms)
- Overhead: 81% slower than sequential
- Computational waste: Exactly 50% as expected

---

## 6. Analysis and Observations

### 6.1 Exploratory Decomposition (N-Queens)

**Advantages:**

- Excellent scalability with good speedup (5.15×)
- Zero wasted computation - all work contributes to final result
- Natural load balancing for symmetric problem spaces
- Minimal synchronization overhead

**Challenges:**

- Load imbalance can occur with irregular solution distributions
- Speedup limited by serial portions (Amdahl's Law)
- Memory overhead for thread-local data structures

### 6.2 Speculative Decomposition (Branch Evaluation)

**Advantages:**

- Eliminates conditional branching delays
- Useful when branch prediction is poor
- Can provide benefits with expensive condition evaluation

**Challenges:**

- Inherent 50% computational waste in this implementation
- Thread synchronization overhead outweighs benefits for simple branches
- Memory and CPU resource overhead
- Poor speedup (0.55×) due to overhead exceeding benefits

### 6.3 Trade-off Analysis

The results clearly demonstrate the fundamental trade-off between the two approaches:

1. **Exploratory Decomposition** achieves high efficiency by ensuring all computation contributes to the final result
2. **Speculative Decomposition** trades computational resources for potentially reduced latency, but overhead can dominate

---

## 7. Conclusions

### 7.1 Key Findings

1. **Exploratory decomposition** is highly effective for problems with:

   - Large, partitionable solution spaces
   - Independent subproblems
   - Minimal synchronization requirements

2. **Speculative decomposition** shows promise but requires:
   - Expensive conditional evaluation to justify overhead
   - Balanced computational cost between branches
   - Careful thread management to minimize overhead

### 7.2 Applicability Guidelines

- **Use Exploratory** for: Search algorithms, backtracking, tree traversal
- **Use Speculative** for: Complex conditional logic, expensive branch conditions, unpredictable control flow

### 7.3 Performance Insights

- The N-Queens problem achieved 43% parallel efficiency, demonstrating good scalability
- Speculative execution overhead can exceed benefits for simple computational branches
- Thread management and synchronization costs must be carefully considered

### 7.4 Future Work

- Implement adaptive load balancing for exploratory decomposition
- Explore work-stealing techniques for irregular workloads
- Investigate hybrid approaches combining both strategies
- Study impact of NUMA architectures on performance

---

## 8. References

1. OpenMP API Specification for Parallel Programming
2. "Introduction to Parallel Computing" - Grama, Gupta, Karypis, Kumar
3. "Parallel Programming: Concepts and Practice" - Schmidt, Gonzalez-Dominguez

---

**Code Repository:** All source code, compilation scripts, and analysis tools are available in the project directory.

**Reproducibility:** Results can be reproduced using the provided `Makefile` and `analyze_performance.py` script.
