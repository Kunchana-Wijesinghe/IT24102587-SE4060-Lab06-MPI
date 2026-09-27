#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/.."

output="Exercise04/timing_runs.csv"
printf 'program,processes,trial,time_seconds\n' > "$output"

for program in sum_mpi pi_monte_carlo; do
    if [ "$program" = "sum_mpi" ]; then
        executable="./Exercise02/sum_mpi"
    else
        executable="./Exercise03/pi_monte_carlo"
    fi

    for processes in 1 2 4; do
        for trial in 1 2 3 4 5 6 7; do
            echo "Running $program with $processes processes (trial $trial/7)"
            result=$(mpirun --oversubscribe -np "$processes" "$executable")
            time=$(printf '%s\n' "$result" | awk '/^Time:/ {print $2}')
            if [ -z "$time" ]; then
                printf '%s\n' "$result"
                exit 1
            fi
            printf '%s,%s,%s,%s\n' \
                "$program" "$processes" "$trial" "$time" >> "$output"
        done
    done
done

echo "Saved results to $output"
