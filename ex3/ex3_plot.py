import matplotlib.pyplot as plt

block_data = {
    'block_size': [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024],
    'time': [39.0653, 12.5920, 4.5817, 3.2765, 3.0012, 3.4170, 4.6308, 4.1767, 4.1539, 4.3076, 11.8012],
    'mflops': [54.9717, 170.5430, 468.7075, 655.4290, 715.5328, 628.4788, 463.7360, 514.1582, 516.9784, 498.5301, 181.9722],
    'bandwidth': [0.6442, 1.9986, 5.4927, 7.6808, 8.3852, 7.3650, 5.4344, 6.0253, 6.0583, 5.8421, 2.1325]
}

fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 5))

ax1.plot(block_data['block_size'], block_data['time'], 'g-o', linewidth=2, markersize=6)
ax1.set_xlabel('Block Size', fontsize=12)
ax1.set_ylabel('Time (s)', fontsize=12)
ax1.set_title('Block Matrix Multiplication: Time', fontsize=14)
ax1.grid(True, alpha=0.3)

ax2.plot(block_data['block_size'], block_data['mflops'], 'm-s', linewidth=2, markersize=6)
ax2.set_xlabel('Block Size', fontsize=12)
ax2.set_ylabel('MFLOPS', fontsize=12)
ax2.set_title('Block Matrix Multiplication: MFLOPS', fontsize=14)
ax2.grid(True, alpha=0.3)

ax3.plot(block_data['block_size'], block_data['bandwidth'], 'c-^', linewidth=2, markersize=6)
ax3.set_xlabel('Block Size', fontsize=12)
ax3.set_ylabel('Bandwidth (MB/s)', fontsize=12)
ax3.set_title('Block Matrix Multiplication: Bandwidth', fontsize=14)
ax3.grid(True, alpha=0.3)

plt.tight_layout()
plt.savefig('block_matrix_multiplication_comparison.png', dpi=150, bbox_inches='tight')
plt.show()
