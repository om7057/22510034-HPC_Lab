import matplotlib.pyplot as plt
import numpy as np


def analyze_matrix_matrix_performance():
    # Read performance data
    processes = []
    times = []

    try:
        with open('matrix_matrix_performance.txt', 'r') as f:
            for line in f:
                if line.strip():
                    p, t = line.strip().split()
                    processes.append(int(p))
                    times.append(float(t))
    except FileNotFoundError:
        print("Performance data file not found. Run the MPI program first.")
        return

    if not processes:
        print("No performance data found.")
        return

    # Calculate metrics
    speedup = [times[0] / t for t in times]
    efficiency = [s / p * 100 for s, p in zip(speedup, processes)]

    # Create plots
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 10))

    # Plot 1: Execution Time
    ax1.plot(processes, times, 'bo-', linewidth=2, markersize=8)
    ax1.set_xlabel('Number of Processes')
    ax1.set_ylabel('Execution Time (seconds)')
    ax1.set_title('Matrix-Matrix Multiplication: Execution Time')
    ax1.grid(True, alpha=0.3)
    ax1.set_yscale('log')

    # Plot 2: Speedup
    ax2.plot(processes, speedup, 'ro-', linewidth=2,
             markersize=8, label='Actual Speedup')
    ax2.plot(processes, processes, 'k--', alpha=0.7, label='Ideal Speedup')
    ax2.set_xlabel('Number of Processes')
    ax2.set_ylabel('Speedup')
    ax2.set_title('Speedup vs Number of Processes')
    ax2.legend()
    ax2.grid(True, alpha=0.3)

    # Plot 3: Efficiency
    ax3.plot(processes, efficiency, 'go-', linewidth=2, markersize=8)
    ax3.set_xlabel('Number of Processes')
    ax3.set_ylabel('Efficiency (%)')
    ax3.set_title('Parallel Efficiency')
    ax3.grid(True, alpha=0.3)
    ax3.set_ylim(0, 110)

    # Plot 4: Performance (GFLOPS)
    N = 500  # Matrix size
    operations = 2 * N**3  # Matrix multiplication operations
    gflops = [(operations / t) / 1e9 for t in times]

    ax4.plot(processes, gflops, 'mo-', linewidth=2, markersize=8)
    ax4.set_xlabel('Number of Processes')
    ax4.set_ylabel('Performance (GFLOPS)')
    ax4.set_title('Computational Performance')
    ax4.grid(True, alpha=0.3)

    plt.tight_layout()
    plt.savefig('matrix_matrix_performance.png', dpi=300, bbox_inches='tight')
    plt.show()

    # Print summary
    print("\n" + "="*60)
    print("MATRIX-MATRIX MULTIPLICATION PERFORMANCE ANALYSIS")
    print("="*60)
    print(f"Matrix Size: 500x500")
    print(f"Total Operations: {operations:,}")
    print("-"*60)
    print("Processes | Time (s) | Speedup | Efficiency | GFLOPS")
    print("-"*60)

    for i, p in enumerate(processes):
        print(
            f"{p:8d} | {times[i]:7.4f} | {speedup[i]:6.2f}x | {efficiency[i]:7.1f}% | {gflops[i]:6.2f}")

    print("-"*60)
    print(
        f"Best Speedup: {max(speedup):.2f}x with {processes[speedup.index(max(speedup))]} processes")
    print(
        f"Best Efficiency: {max(efficiency):.1f}% with {processes[efficiency.index(max(efficiency))]} processes")
    print("="*60)


if __name__ == "__main__":
    analyze_matrix_matrix_performance()
