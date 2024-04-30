#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int ierror, myRank, uniSize, msg, i;
    int runs;
    double start, end;
    MPI_Status status;

    // printf("Input the number of times you would like the programme to ping pong.\n");
    // scanf("%d", runs);

    runs = 5;

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    start = MPI_Wtime();

    msg = 10;

    for (i = 0; i < runs; i++)
    {
        if (myRank == 0)
        {
            msg += 1;
            ierror = MPI_Send(&msg, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            ierror = MPI_Recv(&msg, 1, MPI_INT, 1, 1, MPI_COMM_WORLD, &status);
        }
        else
        {
            ierror = MPI_Recv(&msg, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            msg += 1;
            ierror = MPI_Send(&msg, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }

    end = MPI_Wtime();
    double time = end - start;
    double avgtime = time/runs;

    if (myRank == 0)
    {
        printf("Message value is %d after %d rounds.\n", msg, runs);
        printf("On average, it took %.16f seconds to complete an operation.\n", avgtime);
        printf("MPI_Wtime ticks every %.16f seconds.\n", MPI_Wtick());
    }

    MPI_Finalize();
    return 0;
}