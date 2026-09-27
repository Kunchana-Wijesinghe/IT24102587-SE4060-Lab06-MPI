#include <stdio.h>
#include <stdint.h>
#include <mpi.h>

#define TOTAL_POINTS 10000000LL

/* Produce reproducible pseudo-random values from each point's index. */
static uint64_t mix(uint64_t value) {
    value += UINT64_C(0x9e3779b97f4a7c15);
    value = (value ^ (value >> 30)) * UINT64_C(0xbf58476d1ce4e5b9);
    value = (value ^ (value >> 27)) * UINT64_C(0x94d049bb133111eb);
    return value ^ (value >> 31);
}

static double random_01(uint64_t value) {
    return (mix(value) >> 11) * (1.0 / 9007199254740992.0);
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    long long start = (rank * TOTAL_POINTS) / size;
    long long end = ((rank + 1) * TOTAL_POINTS) / size;
    long long local_inside = 0;
    long long total_inside = 0;

    MPI_Barrier(MPI_COMM_WORLD);
    double t_start = MPI_Wtime();

    for (long long i = start; i < end; i++) {
        uint64_t key = (uint64_t)i * 2;
        double x = random_01(key);
        double y = random_01(key + 1);

        if (x * x + y * y <= 1.0) {
            local_inside++;
        }
    }

    MPI_Reduce(&local_inside, &total_inside, 1, MPI_LONG_LONG_INT,
               MPI_SUM, 0, MPI_COMM_WORLD);

    double elapsed = MPI_Wtime() - t_start;

    if (rank == 0) {
        double pi = 4.0 * total_inside / TOTAL_POINTS;
        printf("Processes: %d\n", size);
        printf("Points: %lld\n", TOTAL_POINTS);
        printf("Inside circle: %lld\n", total_inside);
        printf("Estimated Pi: %.8f\n", pi);
        printf("Time: %.6f seconds\n", elapsed);
    }

    MPI_Finalize();
    return 0;
}
