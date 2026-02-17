import os
import argparse
import pandas as pd
import matplotlib.pyplot as plt

def plot(csv_path, out_dir="plots"):
    df = pd.read_csv(csv_path)
    os.makedirs(out_dir, exist_ok=True)

    for sched in sorted(df['schedule'].unique()):
        sub = df[df['schedule'] == sched]
        chunks = sorted(sub['chunk'].unique())
        plt.figure(figsize=(8,4))
        for c in chunks:
            s = sub[sub['chunk'] == c]
            s = s.sort_values('threads')
            plt.plot(s['threads'], s['speedup'], marker='o', label=f'chunk={c}')
        plt.title(f'Speedup - {sched}')
        plt.xlabel('Threads')
        plt.ylabel('Speedup')
        plt.xticks(s['threads'].unique())
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(out_dir, f'speedup_{sched}.png'))
        plt.close()

        plt.figure(figsize=(8,4))
        for c in chunks:
            s = sub[sub['chunk'] == c]
            s = s.sort_values('threads')
            plt.plot(s['threads'], s['efficiency'], marker='o', label=f'chunk={c}')
        plt.title(f'Efficiency - {sched}')
        plt.xlabel('Threads')
        plt.ylabel('Efficiency')
        plt.xticks(s['threads'].unique())
        plt.grid(True)
        plt.legend()
        plt.tight_layout()
        plt.savefig(os.path.join(out_dir, f'efficiency_{sched}.png'))
        plt.close()

    print(f'Plots written to {out_dir}')

if __name__ == '__main__':

    plot('results.csv', '.')
