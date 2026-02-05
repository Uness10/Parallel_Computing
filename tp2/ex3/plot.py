import matplotlib.pyplot as plt

# Sequential fraction
fs = 0.000042

# Processor counts
p = [1, 2, 4, 8, 16, 32, 64,128,512,1024,2048,4096]

# Theoretical speedup using Amdahl's Law
S = [1 / (fs + (1 - fs) / pi) for pi in p]
SG= [pi - fs * (pi - 1) for pi in p]

# Theoretical maximum speedup
S_max = 1 / fs

# Print table in requested format
print("| p (cores) | Theoretical Speedup S(p) |")
print("|-----------|-------------------------|")
for pi, si in zip(p, S):
    print(f"| {pi:<9}| {si:<25.2f}|")

# Plot
plt.figure()
plt.plot(p, S, marker='o', label='Amdahl Speedup')
plt.axhline(y=S_max, linestyle='--', color='r', label='Theoretical Max Speedup (1/fs)')
plt.plot(p, SG, marker='x', label='Gustafson Speedup')
plt.xlabel('Number of processors (p)')
plt.ylabel('Speedup S(p)')
plt.title('Speedup vs Number of Processors')
plt.legend()
plt.grid(True)
plt.show()
