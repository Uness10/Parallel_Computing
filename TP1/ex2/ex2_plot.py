import matplotlib.pyplot as plt

n = [100, 200, 300, 400, 500, 600, 700, 800, 900, 1000]
time_ijk = [0.010239, 0.047102, 0.131175, 0.303612, 0.661610, 1.155140, 1.851454, 2.796961, 4.124809, 6.489688]
time_ikj = [0.007166, 0.035870, 0.101500, 0.297015, 0.480488, 0.855374, 1.361984, 1.888715, 2.888789, 5.135840]
bandwidth_ijk = [1.46, 2.53, 3.07, 3.14, 2.82, 2.79, 2.76, 2.73, 2.63, 2.30]
bandwidth_ikj = [2.08, 3.32, 3.96, 3.21, 3.88, 3.76, 3.75, 4.04, 3.76, 2.90]

fig2, (ax4, ax5) = plt.subplots(1, 2, figsize=(12, 5))

# Plot time comparison
ax4.plot(n, time_ijk, 'b-o', label='ijk', linewidth=2, markersize=6)
ax4.plot(n, time_ikj, 'r-s', label='ikj', linewidth=2, markersize=6)
ax4.set_xlabel('Matrix Size (n)', fontsize=12)
ax4.set_ylabel('Time (s)', fontsize=12)
ax4.set_title('ijk vs ikj: Time Comparison', fontsize=14)
ax4.legend()
ax4.grid(True, alpha=0.3)

# Plot bandwidth comparison
ax5.plot(n, bandwidth_ijk, 'b-o', label='ijk', linewidth=2, markersize=6)
ax5.plot(n, bandwidth_ikj, 'r-s', label='ikj', linewidth=2, markersize=6)
ax5.set_xlabel('Matrix Size (n)', fontsize=12)
ax5.set_ylabel('Bandwidth (MB/s)', fontsize=12)
ax5.set_title('ijk vs ikj: Bandwidth Comparison', fontsize=14)
ax5.legend()
ax5.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('matrix_multiplication_comparison.png', dpi=150, bbox_inches='tight')
plt.show()