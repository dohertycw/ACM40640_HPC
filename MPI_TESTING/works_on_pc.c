#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char **argv)
{
    int uniSize, myRank, msg;
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    msg = 0;
    if (myRank != 0)
    {
        MPI_Recv(&msg, 1, MPI_INT, myRank - 1, 101,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d from process %d\n",
               myRank, msg, myRank - 1);
    }
    msg += myRank;
    MPI_Send(&msg, 1, MPI_INT, (myRank + 1) % uniSize,
             101, MPI_COMM_WORLD);

    if (myRank == 0)
    {
        MPI_Recv(&msg, 1, MPI_INT, uniSize - 1, 101,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d received %d from process %d\n",
               myRank, msg, uniSize - 1);
    }

    // Finalize the MPI environment.
    MPI_Finalize();
}