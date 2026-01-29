import matplotlib.pyplot as plt

# Data for -O0
strides_O0 = [
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20
]
time_O0 = [
    3.808, 4.162, 4.327, 4.560, 5.322, 5.133, 5.315, 8.314, 15.246, 15.928,
    23.601, 20.350, 34.847, 41.734, 32.061, 26.637, 37.797, 35.067, 33.012, 30.002
]
Bandwidth_O0 = [
    2003.517471, 1833.107768, 1763.206501, 1673.112836, 1433.557785, 1486.342204, 1435.445820, 917.656306, 500.419424, 478.992625,
    323.265732, 374.908822, 218.939781, 182.810048, 237.964958, 286.420938, 201.851854, 217.566217, 231.109734, 254.296198
]

# Data for -O2
strides_O2 = [
    1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 18, 19, 20
]
time_O2 = [
    1.938, 1.619, 1.754, 2.513, 3.260, 4.773, 8.456, 5.653, 5.726, 9.261,
    13.075, 15.606, 17.187, 18.314, 36.656, 28.372, 25.256, 24.969, 23.003, 16.172
]
Bandwidth_O2 = [
    3936.736084, 4712.411693, 4349.711819, 3035.970765, 2340.305071, 1598.448467, 902.246279, 1349.618704, 1332.412597, 823.819731,
    583.510098, 488.875723, 443.904959, 416.588104, 208.134945, 268.905771, 302.082457, 305.554669, 331.669544, 471.765677
]


plt.figure(figsize=(12, 5))

# Plot time vs stride
plt.subplot(1, 2, 1)
plt.plot(strides_O0, time_O0, 'o-', color='red', label='-O0')
plt.plot(strides_O2, time_O2, 's-', color='blue', label='-O2')
plt.xlabel('Stride')
plt.ylabel('Time (ms)')
plt.title('Time vs Stride')
plt.legend()
plt.grid(True)

# Plot Bandwidth vs stride
plt.subplot(1, 2, 2)
plt.plot(strides_O0, Bandwidth_O0, 'o-', color='red', label='-O0')
plt.plot(strides_O2, Bandwidth_O2, 's-', color='blue', label='-O2')
plt.xlabel('Stride')
plt.ylabel('Bandwidth (MB/s)')
plt.title('Bandwidth vs Stride')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
