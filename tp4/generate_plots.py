import matplotlib.pyplot as plt
import numpy as np
import os

# Output directory
out_dir = os.path.join(os.path.dirname(__file__), "plots")
os.makedirs(out_dir, exist_ok=True)

# ===== Data from Exercise 4 runs =====
threads = [1, 2, 4, 8, 16]

# Sequential times (baseline per thread-count run)
t_seq = [1.381, 1.207, 1.225, 1.197, 1.375]

# V1: Implicit barrier (static)
t_v1 = [0.796, 0.530, 0.475, 0.478, 0.543]
s_v1 = [1.73, 2.28, 2.58, 2.51, 2.53]
e_v1 = [1.73, 1.14, 0.64, 0.31, 0.16]
mf_v1 = [60.29, 90.57, 101.01, 100.46, 88.43]

# V3: Static + nowait
t_v3 = [0.712, 0.753, 0.525, 0.569, 0.475]
s_v3 = [1.94, 1.60, 2.34, 2.11, 2.90]
e_v3 = [1.94, 0.80, 0.58, 0.26, 0.18]
mf_v3 = [67.40, 63.71, 91.50, 84.39, 101.05]

# Style settings
plt.rcParams.update({
    'figure.figsize': (8, 5),
    'font.size': 12,
    'axes.grid': True,
    'grid.alpha': 0.3,
})
colors = {'v1': '#2196F3', 'v3': '#FF5722', 'seq': '#4CAF50', 'ideal': '#9E9E9E'}
markers = {'v1': 'o', 'v3': 's'}

# ====================================================
# Plot 1: CPU Time vs Threads
# ====================================================
fig, ax = plt.subplots()
ax.plot(threads, t_v1, marker='o', color=colors['v1'], linewidth=2, label='V1 - Implicit barrier (static)')
ax.plot(threads, t_v3, marker='s', color=colors['v3'], linewidth=2, label='V3 - Static + nowait')
ax.plot(threads, t_seq, marker='^', color=colors['seq'], linewidth=2, linestyle='--', label='Sequential')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('CPU Time (seconds)')
ax.set_title('Exercise 4: CPU Time vs Threads')
ax.set_xticks(threads)
ax.legend()
ax.set_ylim(bottom=0)
plt.tight_layout()
plt.savefig(os.path.join(out_dir, "ex4_cpu_time.png"), dpi=150)
plt.close()

# ====================================================
# Plot 2: Speedup vs Threads
# ====================================================
fig, ax = plt.subplots()
ax.plot(threads, s_v1, marker='o', color=colors['v1'], linewidth=2, label='V1 - Implicit barrier (static)')
ax.plot(threads, s_v3, marker='s', color=colors['v3'], linewidth=2, label='V3 - Static + nowait')
ax.plot(threads, threads, color=colors['ideal'], linewidth=1.5, linestyle=':', label='Ideal speedup')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('Speedup')
ax.set_title('Exercise 4: Speedup vs Threads')
ax.set_xticks(threads)
ax.legend()
ax.set_ylim(bottom=0)
plt.tight_layout()
plt.savefig(os.path.join(out_dir, "ex4_speedup.png"), dpi=150)
plt.close()

# ====================================================
# Plot 3: Efficiency vs Threads
# ====================================================
fig, ax = plt.subplots()
ax.plot(threads, e_v1, marker='o', color=colors['v1'], linewidth=2, label='V1 - Implicit barrier (static)')
ax.plot(threads, e_v3, marker='s', color=colors['v3'], linewidth=2, label='V3 - Static + nowait')
ax.axhline(y=1.0, color=colors['ideal'], linewidth=1.5, linestyle=':', label='Ideal efficiency (1.0)')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('Efficiency (S / P)')
ax.set_title('Exercise 4: Efficiency vs Threads')
ax.set_xticks(threads)
ax.legend()
ax.set_ylim(bottom=0)
plt.tight_layout()
plt.savefig(os.path.join(out_dir, "ex4_efficiency.png"), dpi=150)
plt.close()

# ====================================================
# Plot 4: MFLOP/s vs Threads
# ====================================================
fig, ax = plt.subplots()
ax.plot(threads, mf_v1, marker='o', color=colors['v1'], linewidth=2, label='V1 - Implicit barrier (static)')
ax.plot(threads, mf_v3, marker='s', color=colors['v3'], linewidth=2, label='V3 - Static + nowait')
ax.set_xlabel('Number of Threads')
ax.set_ylabel('MFLOP/s')
ax.set_title('Exercise 4: MFLOP/s vs Threads')
ax.set_xticks(threads)
ax.legend()
ax.set_ylim(bottom=0)
plt.tight_layout()
plt.savefig(os.path.join(out_dir, "ex4_mflops.png"), dpi=150)
plt.close()

# ====================================================
# Plot 5: Exercise 3 - Load Balancing Bar Chart
# ====================================================
fig, ax = plt.subplots(figsize=(8, 5))
versions = ['Sequential', 'Parallel Sections\n(3 sections)', 'Optimized\n(split heavy)']
times = [8.29, 7.04, 2.02]
bar_colors = ['#9E9E9E', '#2196F3', '#4CAF50']
bars = ax.bar(versions, times, color=bar_colors, width=0.5, edgecolor='black', linewidth=0.5)
for bar, t in zip(bars, times):
    ax.text(bar.get_x() + bar.get_width()/2, bar.get_height() + 0.15,
            f'{t:.2f}s', ha='center', va='bottom', fontsize=12, fontweight='bold')
ax.set_ylabel('Execution Time (seconds)')
ax.set_title('Exercise 3: Load Balancing with Parallel Sections')
ax.set_ylim(0, 10)
plt.tight_layout()
plt.savefig(os.path.join(out_dir, "ex3_load_balancing.png"), dpi=150)
plt.close()

print("All plots saved to:", out_dir)
for f in os.listdir(out_dir):
    print(f"  {f}")
