import matplotlib.pyplot as plt
import numpy as np

def analyze_both_programs():
    # Read convolution data
    conv_data = {}
    try:
        with open('convolution_results.txt', 'r') as f:
            for line in f:
                if line.strip():
                    parts = line.strip().split()
                    if len(parts) == 3:
                        try:
                            procs, grid, time = int(parts[0]), int(parts[1]), float(parts[2])
                            if grid not in conv_data:
                                conv_data[grid] = {'procs': [], 'times': []}
                            conv_data[grid]['procs'].append(procs)
                            conv_data[grid]['times'].append(time)
                        except ValueError:
                            print(f"Skipping invalid convolution line: {line.strip()}")
    except FileNotFoundError:
        print("Convolution results not found")
    
    # Read dot product data
    dot_procs, dot_times = [], []
    try:
        with open('dotproduct_results.txt', 'r') as f:
            for line in f:
                if line.strip():
                    parts = line.strip().split()
                    if len(parts) == 2 and parts[1] != '=':
                        try:
                            dot_procs.append(int(parts[0]))
                            dot_times.append(float(parts[1]))
                        except ValueError:
                            print(f"Skipping invalid line: {line.strip()}")
    except FileNotFoundError:
        print("Dot product results not found")
    
    # Create comparison plots
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(15, 12))
    
    # Plot 1: Convolution Performance
    for grid in conv_data:
        times = np.array(conv_data[grid]['times'])
        procs = np.array(conv_data[grid]['procs'])
        ax1.plot(procs, times, 'o-', label=f'{grid}x{grid} grid', linewidth=2, markersize=8)
    
    ax1.set_xlabel('Number of Processes')
    ax1.set_ylabel('Execution Time (seconds)')
    ax1.set_title('2D Convolution Performance')
    ax1.legend()
    ax1.grid(True, alpha=0.3)
    ax1.set_yscale('log')
    
    # Plot 2: Dot Product Performance
    if dot_times:
        ax2.plot(dot_procs, dot_times, 'ro-', linewidth=2, markersize=8)
        ax2.set_xlabel('Number of Processes')
        ax2.set_ylabel('Execution Time (seconds)')
        ax2.set_title('Dot Product Performance (100M elements)')
        ax2.grid(True, alpha=0.3)
        ax2.set_yscale('log')
    
    # Plot 3: Convolution Speedup
    for grid in conv_data:
        times = np.array(conv_data[grid]['times'])
        procs = np.array(conv_data[grid]['procs'])
        speedup = times[0] / times
        ax3.plot(procs, speedup, 'o-', label=f'{grid}x{grid} grid', linewidth=2, markersize=8)
    
    ax3.plot(procs, procs, 'k--', alpha=0.7, label='Ideal Speedup')
    ax3.set_xlabel('Number of Processes')
    ax3.set_ylabel('Speedup')
    ax3.set_title('Convolution Speedup')
    ax3.legend()
    ax3.grid(True, alpha=0.3)
    
    # Plot 4: Dot Product Speedup
    if dot_times:
        speedup = np.array(dot_times[0]) / np.array(dot_times)
        ax4.plot(dot_procs, speedup, 'ro-', linewidth=2, markersize=8, label='Actual')
        ax4.plot(dot_procs, dot_procs, 'k--', alpha=0.7, label='Ideal')
        ax4.set_xlabel('Number of Processes')
        ax4.set_ylabel('Speedup')
        ax4.set_title('Dot Product Speedup')
        ax4.legend()
        ax4.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('mpi_programs_analysis.png', dpi=300, bbox_inches='tight')
    plt.show()
    
    # Print summary
    print("\n" + "="*70)
    print("MPI PROGRAMS PERFORMANCE ANALYSIS")
    print("="*70)
    
    print("\n2D CONVOLUTION RESULTS:")
    print("-" * 50)
    for grid in conv_data:
        print(f"\nGrid Size: {grid}x{grid}")
        times = np.array(conv_data[grid]['times'])
        procs = np.array(conv_data[grid]['procs'])
        speedup = times[0] / times
        efficiency = speedup / procs * 100
        
        for i, p in enumerate(procs):
            print(f"  {p} processes: {times[i]:.4f}s, Speedup: {speedup[i]:.2f}x, Efficiency: {efficiency[i]:.1f}%")
    
    if dot_times:
        print(f"\nDOT PRODUCT RESULTS (100M elements):")
        print("-" * 50)
        speedup = np.array(dot_times[0]) / np.array(dot_times)
        efficiency = speedup / np.array(dot_procs) * 100
        
        for i, p in enumerate(dot_procs):
            print(f"  {p} processes: {dot_times[i]:.4f}s, Speedup: {speedup[i]:.2f}x, Efficiency: {efficiency[i]:.1f}%")

if __name__ == "__main__":
    analyze_both_programs()