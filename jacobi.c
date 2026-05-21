#include <mpi.h>
#include <stdio.h>

#define N 1000
#define ITER 500

int main(int argc, char** argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // --- 1) zaščita pred nedeljivostjo ---
    if (N % size != 0) {
        if (rank == 0)
            printf("Napaka: N (%d) mora biti deljiv s številom procesov (%d)\n", N, size);
        MPI_Finalize();
        return 0;
    }

    int local_n = N / size;
    int start = rank * local_n;
    int end = start + local_n;

    static double A[N][N];
    static double b[N];
    static double x[N];
    static double x_new_local[1000];  // dovolj velik buffer

    // --- 2) inicializacija ---
    for (int i = 0; i < N; i++) {
        b[i] = N + 1;
        x[i] = 0;  // pomembno!
        for (int j = 0; j < N; j++) {
            A[i][j] = (i == j) ? (N + 1) : 1.0;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);
    double start_t = MPI_Wtime();

    // --- 3) Jacobijeva iteracija ---
    for (int iter = 0; iter < ITER; iter++) {

        // vsak proces računa svoj del
        for (int i = start; i < end; i++) {
            double sum = 0.0;

            for (int j = 0; j < N; j++)
                if (j != i)
                    sum += A[i][j] * x[j];

            x_new_local[i - start] = (b[i] - sum) / A[i][i];
        }

        // zberemo vse dele v celoten vektor x
        MPI_Allgather(
            x_new_local,
            local_n,
            MPI_DOUBLE,
            x,
            local_n,
            MPI_DOUBLE,
            MPI_COMM_WORLD
        );
    }

    double end_t = MPI_Wtime();

    if (rank == 0)
        printf("Cas izvajanja: %f s\n", end_t - start_t);

    MPI_Finalize();
    return 0;
}
