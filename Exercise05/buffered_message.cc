#include <cstdio>
#include <vector>
#include <mpi.h>

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 1) {
        int x[10];
        for (int i = 0; i < 10; i++) {
            x[i] = i * 10;
        }

        int packed_size;
        MPI_Pack_size(10, MPI_INT, MPI_COMM_WORLD, &packed_size);
        std::vector<char> buffer(packed_size + MPI_BSEND_OVERHEAD);
        MPI_Buffer_attach(buffer.data(), static_cast<int>(buffer.size()));

        MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);

        std::printf("Rank 1 sent: ");
        for (int value : x) {
            std::printf("%d ", value);
        }
        std::printf("\n");

        void *detached_buffer;
        int detached_size;
        MPI_Buffer_detach(&detached_buffer, &detached_size);
    } else if (rank == 3) {
        int y[10];
        MPI_Status status;
        MPI_Recv(y, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);

        std::printf("Rank 3 received: ");
        for (int value : y) {
            std::printf("%d ", value);
        }
        std::printf("\n");
    }

    MPI_Finalize();
    return 0;
}
