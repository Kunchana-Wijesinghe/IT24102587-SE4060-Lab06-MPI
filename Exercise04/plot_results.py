import csv
from collections import defaultdict
from pathlib import Path
from statistics import median

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt

folder = Path(__file__).resolve().parent
runs = defaultdict(list)

with (folder / "timing_runs.csv").open(newline="") as file:
    for row in csv.DictReader(file):
        key = (row["program"], int(row["processes"]))
        runs[key].append(float(row["time_seconds"]))

names = {
    "sum_mpi": "MPI Sum (1 to 10,000,000)",
    "pi_monte_carlo": "Monte Carlo Pi (10,000,000 points)",
}

with (folder / "summary.csv").open("w", newline="") as file:
    writer = csv.writer(file)
    writer.writerow(["program", "processes", "median_time_seconds", "speedup"])

    for program, title in names.items():
        process_counts = [1, 2, 4]
        times = [median(runs[(program, count)]) for count in process_counts]
        baseline = times[0]
        speedups = [baseline / time for time in times]

        for count, time, speedup in zip(process_counts, times, speedups):
            writer.writerow([program, count, f"{time:.6f}", f"{speedup:.3f}"])

        plt.figure(figsize=(7, 4.5))
        plt.plot(process_counts, times, marker="o", linewidth=2)
        plt.xticks(process_counts)
        plt.xlabel("Number of MPI processes")
        plt.ylabel("Median execution time (seconds)")
        plt.title(f"{title}: Time vs Processes")
        plt.grid(alpha=0.3)
        plt.tight_layout()
        plt.savefig(folder / f"{program}_time.png", dpi=180)
        plt.close()

        plt.figure(figsize=(7, 4.5))
        plt.plot(process_counts, speedups, marker="o", linewidth=2,
                 label="Measured speedup")
        plt.plot(process_counts, process_counts, "--", alpha=0.6,
                 label="Ideal speedup")
        plt.xticks(process_counts)
        plt.xlabel("Number of MPI processes")
        plt.ylabel("Speedup (T1 / Tp)")
        plt.title(f"{title}: Speedup")
        plt.grid(alpha=0.3)
        plt.legend()
        plt.tight_layout()
        plt.savefig(folder / f"{program}_speedup.png", dpi=180)
        plt.close()

print("Saved summary.csv and four PNG graphs in Exercise04/")
