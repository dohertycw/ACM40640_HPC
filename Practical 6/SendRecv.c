#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    int ierror, myRank, uniSize;
    char *msg;
    MPI_Status status;

    msg = "Hello world!";

    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    if (myRank == 0)
    {
        ierror = MPI_Send(&msg, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    }else{
        ierror = MPI_Recv(&msg, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        printf("%s from process %d\n", msg, myRank);
    }

    ierror = MPI_Finalize();
}