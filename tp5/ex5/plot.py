import csv
import matplotlib.pyplot as plt
from collections import defaultdict

data = defaultdict(lambda: {'P': [], 'speedup': [], 'efficiency': []})

with open('timings.csv', 'r') as f:
    reader = csv.reader(f)
    for row in reader:
        if not row:
            continue
        N = int(row[0])
        P = int(row[1])
        speedup = float(row[4])
        efficiency = float(row[5])
        
        data[N]['P'].append(P)
        data[N]['speedup'].append(speedup)
        data[N]['efficiency'].append(efficiency)

for N in data:
    sorted_indices = sorted(range(len(data[N]['P'])), key=lambda k: data[N]['P'][k])
    data[N]['P'] = [data[N]['P'][i] for i in sorted_indices]
    data[N]['speedup'] = [data[N]['speedup'][i] for i in sorted_indices]
    data[N]['efficiency'] = [data[N]['efficiency'][i] for i in sorted_indices]

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 5))

max_P = 0

for N, values in data.items():
    ax1.plot(values['P'], values['speedup'], marker='o', label=f'N={N}')
    if max(values['P']) > max_P:
        max_P = max(values['P'])

if max_P > 0:
    ax1.plot([1, max_P], [1, max_P], 'k--', label='Ideal Speedup')

ax1.set_xlabel('Number of Processes (P)')
ax1.set_ylabel('Speedup')
ax1.set_title('Speedup vs Number of Processes')
ax1.legend()
ax1.grid(True)

for N, values in data.items():
    ax2.plot(values['P'], values['efficiency'], marker='o', label=f'N={N}')

ax2.axhline(y=1.0, color='k', linestyle='--', label='Ideal Efficiency')

ax2.set_xlabel('Number of Processes (P)')
ax2.set_ylabel('Efficiency')
ax2.set_title('Efficiency vs Number of Processes')
ax2.legend()
ax2.grid(True)

plt.tight_layout()
plt.savefig('speedup_efficiency.png')
print("Plot saved as speedup_efficiency.png")
plt.show()
