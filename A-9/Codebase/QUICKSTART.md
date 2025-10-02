# Quick Start Guide - HPC Practical No. 9

## Files Created:

1. `exploratory_nqueens.c` - N-Queens with exploratory decomposition
2. `speculative_branch.c` - Branch evaluation with speculative decomposition
3. `Makefile` - Build automation
4. `analyze_performance.py` - Performance analysis script
5. `REPORT.md` - Complete technical report template
6. `README.md` - Detailed documentation

## How to Run:

### Option 1: Quick execution

```bash
make run
```

### Option 2: Individual programs

```bash
make all
./exploratory_nqueens
./speculative_branch
```

### Option 3: Performance analysis

```bash
python3 analyze_performance.py
```

## Expected Results:

- N-Queens: ~5x speedup, 0% waste
- Branch Evaluation: ~0.5x speedup, 50% waste

## For Your Report:

1. Use `REPORT.md` as template
2. Include results from `results.csv`
3. Add your personal analysis
4. Modify conclusions based on your observations

## Key Learning Points:

- Exploratory: Good for search problems, no wasted work
- Speculative: Good for complex conditions, trades resources for speed
- OpenMP: Easy parallelization with `#pragma omp`
- Trade-offs: Overhead vs. benefits analysis

The implementation is complete and ready for submission!
