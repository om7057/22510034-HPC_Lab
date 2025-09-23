import matplotlib.pyplot as plt

# Read data
processes = []
times = []

try:
    with open('simple_performance.txt', 'r') as f:
        for line in f:
            p, t = line.strip().split()
            processes.append(int(p))
            times.append(float(t))
except FileNotFoundError:
    print("No performance data found. Run the MPI program first.")
    exit()

# Plot
plt.figure(figsize=(10, 6))

plt.subplot(1, 2, 1)
plt.plot(processes, times, 'bo-', linewidth=2, markersize=8)
plt.xlabel('Number of Processes')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time vs Processes')
plt.grid(True)

plt.subplot(1, 2, 2)
speedup = [times[0]/t for t in times]
plt.plot(processes, speedup, 'ro-', linewidth=2, markersize=8, label='Actual')
plt.plot(processes, processes, 'k--', label='Ideal')
plt.xlabel('Number of Processes')
plt.ylabel('Speedup')
plt.title('Speedup vs Processes')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.savefig('simple_performance.png')
plt.show()

print("Performance Data:")
for i, p in enumerate(processes):
    print(f"{p} processes: {times[i]:.6f}s, Speedup: {speedup[i]:.2f}x")
