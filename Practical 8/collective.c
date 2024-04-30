#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>

int min(int a, int b)
{
    if (a > b)
    {
        return b;
    }
    return a;
}

int main(int argc, char **argv)
{
    int i, m, ierror, myRank, uniSize;
    char msg;
    char *msgs;

    m = atoi(argv[1]);

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    ierror = MPI_Barrier(MPI_COMM_WORLD);
    ierror = MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    ierror = MPI_Barrier(MPI_COMM_WORLD);

    if (myRank != 0)
    {
        printf("Message received on rank %d: %d\n", myRank, m);
    }

    int val = rand() % (123 - 97) + 97;
    msg = val;

    if (myRank == 0)
    {
        msgs = (char *)malloc(sizeof(char) * uniSize);
    }

    ierror = MPI_Gather(&msg, 1, MPI_CHAR, msgs, 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    if (myRank == 0)
    {
        for (i = 0; i < uniSize; i++)
        {
            printf("From rank %d: %c\n", i, msgs[i]);
        }
    }

    int chunk = m / uniSize;
    int start = myRank * chunk;
    int end = min((myRank + 1) * chunk, m);
    if (myRank == uniSize - 1)
    {
        end = m;
    }

    int length = end - start;

    double *a = (double *)malloc(length * sizeof(double));

    for (i = 0; i < length; i++)
    {
        a[i] = (double)(i);
    }

    double s = 0.0, sum = 0.0;
    for (i = 0; i < length; i++)
    {
        s += a[i];
    }

    ierror = MPI_Reduce(&s, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myRank == 0)
    {
        printf("Total sum = %.16f\n", sum);
    }

    free(a);

    MPI_Finalize();
    return ierror;
}