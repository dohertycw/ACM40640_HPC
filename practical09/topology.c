#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <omp.h>
#include <math.h>
#include "alloc2d1.h"

int main(int argc, char **argv)
{
    int ierror, worldRank, worldSize;
    int cartRank, cartSize, periods[2];
    int reorder;
    MPI_Comm comm_cart;

    ierror = MPI_Init(&argc, &argv);

    ierror = MPI_Comm_size(MPI_COMM_WORLD, &worldSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &worldRank);

    int ndims = 2;
    int dims[2] = {0,0};

    ierror = MPI_Dims_create(worldSize, ndims, dims);

    // if(worldRank==0)
    // {
    //     printf("dims array: %d %d \n", dims[0], dims[1]);
    // }

    periods = {1,1};
    reorder = 0;

    ierror = MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, reorder, &comm_cart);

    ierror = MPI_Finalize();

    return ierror;
}