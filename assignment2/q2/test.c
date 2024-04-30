#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int i, j, k, col, row, rank, ierror,unisize;

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &unisize);
    printf("%d\n", unisize);
    ierror = MPI_Finalize();
}