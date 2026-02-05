import matplotlib.pyplot as plt

# Table 1: Unroll Factor vs Time (ms)
unroll_factors_1 = list(range(1, 33))
times_1 = [
    3.171, 2.204, 1.488, 1.327, 1.438, 1.216, 1.171, 1.078, 1.163, 1.249, 1.265, 1.278,
    1.189, 1.285, 1.195, 1.190, 1.466, 1.801, 2.177, 1.328, 1.197, 2.966, 3.642, 1.424,
    1.833, 2.554, 3.445, 3.487, 1.675, 1.358, 1.499, 1.790
]

# Table 4: -O0 and -O2
times_O0 = [
    4.203, 2.344, 2.078, 6.106, 2.258, 2.272, 3.312, 2.730, 1.933, 1.216, 1.620, 1.292,
    1.229, 1.242, 1.912, 1.930, 2.304, 3.770, 3.373, 1.628, 1.683, 1.441, 1.797, 1.889,
    5.506, 2.534, 2.691, 1.974, 2.212, 4.868, 2.755, 2.663
]
times_O2 = [
    2.104, 1.543, 0.908, 0.801, 0.679, 0.654, 0.807, 0.941, 1.006, 0.907, 2.851, 4.322,
    2.064, 1.490, 2.576, 1.211, 1.860, 1.162, 0.908, 0.983, 1.163, 0.966, 0.982, 1.001,
    1.012, 0.970, 0.944, 0.856, 0.657, 0.655, 1.096, 1.955
]

# Table 6.1: Float
times_float = [
    3.058, 2.136, 1.687, 1.287, 1.215, 1.161, 1.190, 2.056, 1.672, 1.018, 0.941, 1.252,
    2.124, 2.115, 2.707, 2.078, 2.373, 2.180, 3.201, 1.445, 1.102, 1.328, 1.384, 1.521,
    1.321, 1.662, 3.408, 2.530, 1.542, 1.551, 1.872, 1.823
]

# Table 6.2: Int
times_int = [
    2.611, 1.579, 1.337, 1.212, 1.266, 1.419, 0.988, 0.981, 0.854, 0.935, 0.945, 1.061,
    1.476, 2.026, 1.018, 0.839, 1.074, 1.058, 1.041, 1.323, 1.763, 0.954, 0.947, 1.003,
    1.047, 1.835, 2.410, 1.356, 1.129, 1.178, 1.148, 1.517
]

# Table 6.3: Short (N=1e4)
times_short = [
    0.021, 0.014, 0.010, 0.067, 0.011, 0.029, 0.013, 0.013, 0.034, 0.012, 0.015, 0.011,
    0.014, 0.008, 0.008, 0.014, 0.037, 0.034, 0.012, 0.012, 0.036, 0.012, 0.013, 0.013,
    0.013, 0.013, 0.027, 0.011, 0.032, 0.011, 0.112, 0.010
]

plt.figure(figsize=(12, 8))

# Plot 1: Manual Unrolling (Table 1)
plt.subplot(2, 2, 1)
plt.plot(unroll_factors_1, times_1, marker='o', label='Manual Unrolling')
plt.xlabel('Unroll Factor (U)')
plt.ylabel('Time (ms)')
plt.title('Manual Unrolling (Table 1)')
plt.grid(True)
plt.legend()

# Plot 2: -O0 vs -O2
plt.subplot(2, 2, 2)
plt.plot(unroll_factors_1, times_O0, marker='o', label='-O0')
plt.plot(unroll_factors_1, times_O2, marker='s', label='-O2')
plt.xlabel('Unroll Factor (U)')
plt.ylabel('Time (ms)')
plt.title('Manual Unrolling: -O0 vs -O2')
plt.grid(True)
plt.legend()

# Plot 3: Float, Int, Short
plt.subplot(2, 2, 3)
plt.plot(unroll_factors_1, times_float, marker='o', label='Float')
plt.plot(unroll_factors_1, times_int, marker='s', label='Int')
plt.plot(unroll_factors_1, times_short, marker='^', label='Short (N=1e4)')
plt.xlabel('Unroll Factor (U)')
plt.ylabel('Time (ms)')
plt.title('Manual Unrolling: Float, Int, Short')
plt.grid(True)
plt.legend()

plt.tight_layout()
plt.show()