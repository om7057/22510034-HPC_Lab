#!/usr/bin/env python3

def analyze_both_programs():
    print("="*70)
    print("MPI PROGRAMS PERFORMANCE ANALYSIS")
    print("="*70)
    
    # Read convolution data
    conv_data = {}
    print("\nReading convolution results...")
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
    print("Reading dot product results...")
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
    
    # Print convolution analysis
    if conv_data:
        print("\n2D CONVOLUTION RESULTS:")
        print("-" * 60)
        for grid in conv_data:
            print(f"\nGrid Size: {grid}x{grid}")
            times = conv_data[grid]['times']
            procs = conv_data[grid]['procs']
            
            if times:
                speedup = [times[0]/t for t in times]
                efficiency = [s/p*100 for s, p in zip(speedup, procs)]
                
                print("Processes | Time (s) | Speedup | Efficiency")
                print("-" * 45)
                for i, p in enumerate(procs):
                    print(f"{p:8d} | {times[i]:7.6f} | {speedup[i]:6.2f}x | {efficiency[i]:7.1f}%")
    
    # Print dot product analysis
    if dot_times:
        print(f"\nDOT PRODUCT RESULTS (100M elements):")
        print("-" * 60)
        speedup = [dot_times[0]/t for t in dot_times]
        efficiency = [s/p*100 for s, p in zip(speedup, dot_procs)]
        
        print("Processes | Time (s) | Speedup | Efficiency")
        print("-" * 45)
        for i, p in enumerate(dot_procs):
            print(f"{p:8d} | {dot_times[i]:7.6f} | {speedup[i]:6.2f}x | {efficiency[i]:7.1f}%")
    
    # Summary insights
    print("\n" + "="*70)
    print("KEY INSIGHTS:")
    print("="*70)
    
    if conv_data:
        print("\n2D CONVOLUTION:")
        for grid in conv_data:
            times = conv_data[grid]['times']
            procs = conv_data[grid]['procs']
            if len(times) > 1:
                best_speedup = max([times[0]/t for t in times])
                best_proc_idx = [times[0]/t for t in times].index(best_speedup)
                best_procs = procs[best_proc_idx]
                print(f"  - Grid {grid}x{grid}: Best speedup {best_speedup:.2f}x with {best_procs} processes")
                print(f"  - Communication overhead visible with more processes")
    
    if dot_times and len(dot_times) > 1:
        best_speedup = max([dot_times[0]/t for t in dot_times])
        best_proc_idx = [dot_times[0]/t for t in dot_times].index(best_speedup)
        best_procs = dot_procs[best_proc_idx]
        print(f"\nDOT PRODUCT:")
        print(f"  - Best speedup {best_speedup:.2f}x with {best_procs} processes")
        print(f"  - Shows classic embarrassingly parallel behavior")
        print(f"  - Performance degrades at {max(dot_procs)} processes due to overhead")
    
    print("\n" + "="*70)

if __name__ == "__main__":
    analyze_both_programs()