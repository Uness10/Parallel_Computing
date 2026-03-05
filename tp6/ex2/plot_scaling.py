#!/usr/bin/env python3
"""
Speedup and Efficiency Plot Generator for Distributed Gradient Descent

This script reads scaling results from a CSV file and generates:
1. Speedup plot
2. Efficiency plot

Usage:
    python plot_scaling.py [results_file.csv]

If no file is provided, it uses 'scaling_results.csv' by default.
"""

import matplotlib.pyplot as plt
import numpy as np
import sys
import os

def read_results(filename):
    """Read scaling results from CSV file."""
    processes = []
    times = []
    
    with open(filename, 'r') as f:
        # Skip header
        header = f.readline()
        
        for line in f:
            parts = line.strip().split(',')
            if len(parts) >= 2:
                processes.append(int(parts[0]))
                times.append(float(parts[1]))
    
    return np.array(processes), np.array(times)

def calculate_metrics(processes, times):
    """Calculate speedup and efficiency."""
    t1 = times[0]  # Time with 1 process (baseline)
    
    # Speedup = T(1) / T(p)
    speedup = t1 / times
    
    # Efficiency = Speedup / p = T(1) / (p * T(p))
    efficiency = speedup / processes
    
    return speedup, efficiency

def plot_speedup(processes, speedup, output_file='speedup.png'):
    """Generate speedup plot."""
    plt.figure(figsize=(10, 6))
    
    # Plot actual speedup
    plt.plot(processes, speedup, 'bo-', linewidth=2, markersize=8, label='Measured Speedup')
    
    # Plot ideal speedup (linear)
    plt.plot(processes, processes, 'r--', linewidth=2, label='Ideal Speedup')
    
    plt.xlabel('Number of Processes', fontsize=12)
    plt.ylabel('Speedup', fontsize=12)
    plt.title('Speedup of Distributed Gradient Descent', fontsize=14)
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.3)
    
    # Set axis limits
    plt.xlim(0, max(processes) + 2)
    plt.ylim(0, max(max(speedup), max(processes)) * 1.1)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=150)
    plt.close()
    print(f"Speedup plot saved to: {output_file}")

def plot_efficiency(processes, efficiency, output_file='efficiency.png'):
    """Generate efficiency plot."""
    plt.figure(figsize=(10, 6))
    
    # Plot actual efficiency
    plt.plot(processes, efficiency * 100, 'go-', linewidth=2, markersize=8, label='Measured Efficiency')
    
    # Plot ideal efficiency (100%)
    plt.axhline(y=100, color='r', linestyle='--', linewidth=2, label='Ideal Efficiency (100%)')
    
    plt.xlabel('Number of Processes', fontsize=12)
    plt.ylabel('Efficiency (%)', fontsize=12)
    plt.title('Efficiency of Distributed Gradient Descent', fontsize=14)
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.3)
    
    # Set axis limits
    plt.xlim(0, max(processes) + 2)
    plt.ylim(0, 110)
    
    plt.tight_layout()
    plt.savefig(output_file, dpi=150)
    plt.close()
    print(f"Efficiency plot saved to: {output_file}")

def plot_combined(processes, speedup, efficiency, output_file='scaling_combined.png'):
    """Generate combined speedup and efficiency plot."""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(14, 5))
    
    # Speedup subplot
    ax1.plot(processes, speedup, 'bo-', linewidth=2, markersize=8, label='Measured')
    ax1.plot(processes, processes, 'r--', linewidth=2, label='Ideal')
    ax1.set_xlabel('Number of Processes', fontsize=12)
    ax1.set_ylabel('Speedup', fontsize=12)
    ax1.set_title('Speedup', fontsize=14)
    ax1.legend(fontsize=10)
    ax1.grid(True, alpha=0.3)
    ax1.set_xlim(0, max(processes) + 2)
    
    # Efficiency subplot
    ax2.plot(processes, efficiency * 100, 'go-', linewidth=2, markersize=8, label='Measured')
    ax2.axhline(y=100, color='r', linestyle='--', linewidth=2, label='Ideal (100%)')
    ax2.set_xlabel('Number of Processes', fontsize=12)
    ax2.set_ylabel('Efficiency (%)', fontsize=12)
    ax2.set_title('Efficiency', fontsize=14)
    ax2.legend(fontsize=10)
    ax2.grid(True, alpha=0.3)
    ax2.set_xlim(0, max(processes) + 2)
    ax2.set_ylim(0, 110)
    
    plt.suptitle('Scaling Analysis of Distributed Gradient Descent', fontsize=16, y=1.02)
    plt.tight_layout()
    plt.savefig(output_file, dpi=150, bbox_inches='tight')
    plt.close()
    print(f"Combined plot saved to: {output_file}")

def print_summary(processes, times, speedup, efficiency):
    """Print summary table of results."""
    print("\n" + "="*70)
    print("SCALING RESULTS SUMMARY")
    print("="*70)
    print(f"{'Processes':>10} {'Time (s)':>12} {'Speedup':>12} {'Efficiency':>12}")
    print("-"*70)
    
    for i in range(len(processes)):
        print(f"{processes[i]:>10} {times[i]:>12.4f} {speedup[i]:>12.2f} {efficiency[i]*100:>11.1f}%")
    
    print("="*70)
    print(f"Maximum speedup achieved: {max(speedup):.2f}x with {processes[np.argmax(speedup)]} processes")
    print(f"Best efficiency: {max(efficiency)*100:.1f}% with {processes[np.argmax(efficiency)]} process(es)")
    print("="*70 + "\n")

def generate_sample_data():
    """Generate sample data for demonstration if no results file exists."""
    # Simulated data for demonstration
    processes = np.array([1, 2, 4, 7, 8, 14, 16, 28, 32, 48, 56])
    
    # Simulate realistic times with diminishing returns
    base_time = 10.0  # seconds for 1 process
    overhead = 0.1    # communication overhead per process
    serial_fraction = 0.05  # 5% serial code
    
    times = []
    for p in processes:
        # Amdahl's law with overhead
        parallel_time = base_time * ((serial_fraction) + (1 - serial_fraction) / p)
        comm_overhead = overhead * np.log2(p + 1)
        times.append(parallel_time + comm_overhead)
    
    return processes, np.array(times)

def main():
    # Get results file from command line or use default
    if len(sys.argv) > 1:
        results_file = sys.argv[1]
    else:
        results_file = 'scaling_results.csv'
    
    # Check if results file exists
    if os.path.exists(results_file):
        print(f"Reading results from: {results_file}")
        processes, times = read_results(results_file)
    else:
        print(f"Results file '{results_file}' not found.")
        print("Generating sample data for demonstration...")
        processes, times = generate_sample_data()
        
        # Save sample data
        with open('scaling_results_sample.csv', 'w') as f:
            f.write("processes,time,samples\n")
            for p, t in zip(processes, times):
                f.write(f"{p},{t:.6f},100000\n")
        print("Sample data saved to: scaling_results_sample.csv")
    
    # Calculate metrics
    speedup, efficiency = calculate_metrics(processes, times)
    
    # Print summary
    print_summary(processes, times, speedup, efficiency)
    
    # Generate plots
    plot_speedup(processes, speedup)
    plot_efficiency(processes, efficiency)
    plot_combined(processes, speedup, efficiency)
    
    print("\nAll plots generated successfully!")

if __name__ == "__main__":
    main()
