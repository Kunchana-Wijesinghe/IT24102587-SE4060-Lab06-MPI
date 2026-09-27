#include <stdio.h>
#include <mpi.h>

#define N 10000000LL

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* Divide 1..N among all processes. */
    long long start = (rank * N) / size + 1;
    long long end = ((rank + 1) * N) / size;
    long long local_sum = 0;
    long long total_sum = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    for (long long number = start; number <= end; number++) {
        local_sum += number;
    }

    MPI_Reduce(&local_sum, &total_sum, 1, MPI_LONG_LONG_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - t_start;

    if (rank == 0) {
        long long expected = N * (N + 1) / 2;
        printf("Processes: %d\n", size);
        printf("Sum: %lld\n", total_sum);
        printf("Expected: %lld\n", expected);
        printf("Correct: %s\n", total_sum == expected ? "YES" : "NO");
        printf("Time: %.6f seconds\n", elapsed);
    }

    MPI_Finalize();
    return 0;
}
