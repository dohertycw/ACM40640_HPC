#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int myRank, uniSize, ierror;

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    printf("I am %d of %d \n", myRank, uniSize);
    ierror = MPI_Finalize();

    return 0;
}
