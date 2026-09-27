# SE4060 Lab 06 - MPI

Student ID: IT24102587

## Environment

Ubuntu on WSL, Open MPI, GCC/G++, Python 3, and Matplotlib.

All tests below ran as MPI processes on one computer. A separate multi-node
cluster is required to demonstrate execution across multiple physical nodes.

## Exercises

- Exercise 0/1: Compiled and ran the supplied HelloMPI and message examples.
- Exercise 2: Parallel sum of integers 1 to 10,000,000 using MPI_Reduce.
  Correct sum: 50000005000000.
- Exercise 3: Monte Carlo estimation of Pi using 10,000,000 points.
  Estimated Pi: 3.14161920.
- Exercise 4: Seven timing runs per program and process count (1, 2, 4).
  summary.csv contains median times and speedups. Four PNG graphs are included.
- Exercise 5: Tested a mismatched receive source and rewrote the array
  message example using MPI_Bsend.
- Exercise 6: Rewrote Exercise 3 using MPI_Recv with MPI_ANY_SOURCE.
- Exercise 7: Rewrote Exercise 6 using MPI_Bsend for worker results.

## Exercise 5 observation

In source_mismatch.cc, rank 1 sends to rank 3, but rank 3 waits for a message
from rank 2. The messages do not match, so the program does not finish.
The test was stopped after five seconds with timeout (exit code 124).
buffered_message.cc sends the array using MPI_Bsend without changing
the array used for sending.

## Benchmark results

| Program | Processes | Median time (s) | Speedup |
|---|---:|---:|---:|
| Sum | 1 | 0.002545 | 1.000 |
| Sum | 2 | 0.001638 | 1.554 |
| Sum | 4 | 0.001499 | 1.698 |
| Monte Carlo Pi | 1 | 0.043358 | 1.000 |
| Monte Carlo Pi | 2 | 0.033632 | 1.289 |
| Monte Carlo Pi | 4 | 0.021789 | 1.990 |

Speedup = median time with 1 process / median time with P processes.
Times were measured on one WSL computer and can vary between runs.

## Build and run

From the repository root:

```bash
mpicc -O2 -Wall -Wextra -std=c11 Exercise02/sum_mpi.c -o Exercise02/sum_mpi
mpicc -O2 -Wall -Wextra -std=c11 Exercise03/pi_monte_carlo.c -o Exercise03/pi_monte_carlo
mpicc -O2 -Wall -Wextra -std=c11 Exercise06/pi_any_source.c -o Exercise06/pi_any_source
mpicc -O2 -Wall -Wextra -std=c11 Exercise07/pi_buffered_send.c -o Exercise07/pi_buffered_send
mpicxx -Wall -Wextra -std=c++11 Exercise05/buffered_message.cc -o Exercise05/buffered_message

mpirun --oversubscribe -np 4 ./Exercise02/sum_mpi
mpirun --oversubscribe -np 4 ./Exercise03/pi_monte_carlo
mpirun --oversubscribe -np 4 ./Exercise05/buffered_message
mpirun --oversubscribe -np 4 ./Exercise06/pi_any_source
mpirun --oversubscribe -np 4 ./Exercise07/pi_buffered_send
./Exercise04/benchmark.sh
python3 Exercise04/plot_results.py

### 2. Compiled executables GitHub එකට නොයන ලෙස සකස් කරන්න

```bash
cat >> .gitignore <<'EOF'
Exercise02/sum_mpi
Exercise03/pi_monte_carlo
Exercise05/source_mismatch
Exercise05/buffered_message
Exercise06/pi_any_source
Exercise07/pi_buffered_send
