# Makefile for HPC Practical No. 9
# Exploratory & Speculative Decomposition

CC = gcc
CFLAGS = -fopenmp -O2 -Wall
LIBS = -lm

# Target executables
TARGETS = exploratory_nqueens speculative_branch

# Default target
all: $(TARGETS)

# Build exploratory decomposition program
exploratory_nqueens: exploratory_nqueens.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "✓ Exploratory N-Queens compiled successfully"

# Build speculative decomposition program
speculative_branch: speculative_branch.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
	@echo "✓ Speculative Branch Evaluation compiled successfully"

# Run both programs
run: all
	@echo "=== Running Exploratory Decomposition (N-Queens) ==="
	./exploratory_nqueens
	@echo ""
	@echo "=== Running Speculative Decomposition (Branch Evaluation) ==="
	./speculative_branch

# Clean compiled files
clean:
	rm -f $(TARGETS)
	@echo "Cleaned compiled files"

# Check OpenMP support
check-openmp:
	@echo "Checking OpenMP support..."
	@$(CC) --version
	@echo | $(CC) -fopenmp -dM -E - | grep -i openmp || echo "OpenMP not found"

# Display system info
system-info:
	@echo "System Information:"
	@echo "CPU cores: $$(nproc)"
	@echo "Current threads setting: $${OMP_NUM_THREADS:-default}"

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Compile both programs"
	@echo "  run          - Compile and run both programs"
	@echo "  clean        - Remove compiled files"
	@echo "  check-openmp - Check OpenMP compiler support"
	@echo "  system-info  - Display system information"
	@echo "  help         - Show this help message"

.PHONY: all run clean check-openmp system-info help