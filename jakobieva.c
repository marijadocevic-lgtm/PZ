#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000
#define ITER 500

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (N % size != 0) {
        if (rank == 0)
            printf("Napaka: N (%d) mora biti deljiv s številom procesov (%d)\n", N, size);
        MPI_Finalize();
        return 0;
    }

    int local_n = N / size;

    // --- Alokacija lokalnih podatkov ---
    double* A_local = malloc(local_n * N * sizeof(double));
    double* b_local = malloc(local_n * sizeof(double));
    double* x = malloc(N * sizeof(double));
    double* x_new_local = malloc(local_n * sizeof(double));

    // --- Inicializacija samo na rank 0 ---
    double* A = NULL;
    double* b = NULL;

    if (rank == 0) {
        A = malloc(N * N * sizeof(double));
        b = malloc(N * sizeof(double));

        for (int i = 0; i < N; i++) {
            b[i] = N + 1;
            for (int j = 0; j < N; j++) {
                A[i*N + j] = (i == j) ? (N + 1) : 1.0;
            }
        }
    }

    // --- Scatter matrike A in vektorja b ---
    MPI_Scatter(A, local_n * N, MPI_DOUBLE,
                A_local, local_n * N, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    MPI_Scatter(b, local_n, MPI_DOUBLE,
                b_local, local_n, MPI_DOUBLE,
                0, MPI_COMM_WORLD);

    // --- Inicializacija x ---
    for (int i = 0; i < N; i++)
        x[i] = 0.0;

    MPI_Barrier(MPI_COMM_WORLD);
    double start_t = MPI_Wtime();

    // --- Jacobi iteracije ---
    for (int iter = 0; iter < ITER; iter++) {

        // Broadcast trenutnega vektorja x
        MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Izračun lokalnega dela
        for (int i = 0; i < local_n; i++) {
            double sum = 0.0;
            int global_i = rank * local_n + i;

            for (int j = 0; j < N; j++) {
                if (j != global_i)
                    sum += A_local[i*N + j] * x[j];
            }

            x_new_local[i] = (b_local[i] - sum) / A_local[i*N + global_i];
        }

        // Gather novega vektorja x
        MPI_Gather(x_new_local, local_n, MPI_DOUBLE,
                   x, local_n, MPI_DOUBLE,
                   0, MPI_COMM_WORLD);
    }

    double end_t = MPI_Wtime();

    if (rank == 0)
        printf("Cas izvajanja (Scatter/Bcast/Gather): %f s\n", end_t - start_t);

    free(A_local);
    free(b_local);
    free(x);
    free(x_new_local);

    if (rank == 0) {
        free(A);
        free(b);
    }

    MPI_Finalize();
    return 0;
}
/*  mpicc jakobieva.c -o jacobi2
    mpirun -np 4 ./jacobi2
for p in 1 2 4 8; do
    echo "===== np = $p ====="
    for r in 1 2 3; do
        echo "Run $r:"
        mpirun --oversubscribe -np $p ./jacobi2
    done */
