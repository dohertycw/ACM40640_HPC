#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>
#include <omp.h>

int main(int argc, char **argv)
{
    int ierror, myRank, uniSize, i;
    int runs;
    double start, end;
    MPI_Status status;

    // printf("Input the number of times you would like the programme to ping pong.\n");
    // scanf("%d", runs);

    runs = 10000;

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    double *msg = (double *)malloc(sizeof(double)*2);

    for (i = 0; i < 2; i++)
    {
        msg[i] = 0.0;
    }
    

    start = omp_get_wtime();

    for (i = 0; i < runs; i++)
    {
        if (myRank == 0)
        {
            ierror = MPI_Send(&msg, 2, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
            ierror = MPI_Recv(&msg, 2, MPI_DOUBLE, 1, 1, MPI_COMM_WORLD, &status);
        }
        else
        {
            ierror = MPI_Recv(&msg, 2, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
            ierror = MPI_Send(&msg, 2, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        }
    }

    end = omp_get_wtime(); // Jake says that, by and large, omp_get_wtime from openmp will provide a better result, so use that instead.
    double time = end - start;
    double avgtime = time/runs;

    if (myRank == 0)
    {
        printf("On average, there was a latency of %.16fs.\n", avgtime);
        printf("Average bandwith was %.16f bytes/s.\n", 2*sizeof(double)/avgtime);
    }

    MPI_Finalize();
    return ierror;
}