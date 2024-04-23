#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

int main(int argc, char **argv)
{
    // Initializing variables
    int ierror, myRank, uniSize, msg;
    MPI_Status status;

    // Initializing MPI.
    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // Part 1 - Blocking communication

    // Setting the message to zero initially.
    msg = 0;

    // Checking if we're on process 0. If not, receive from the previous processor.
    if (myRank != 0)
    {
        ierror = MPI_Recv(&msg, 1, MPI_INT, myRank - 1, 101, MPI_COMM_WORLD, &status);
    }

    // Increments the message by the processor's rank.
    msg += myRank;

    // Send the message to the next processor. Using modulo here prevents us from going
    // over the amount of processors we've defined.
    ierror = MPI_Send(&msg, 1, MPI_INT, (myRank + 1) % uniSize, 101, MPI_COMM_WORLD);

    // Finally, once the message has gone from processor to processor and looped back to processor 0,
    // print the message it's received.
    if (myRank == 0)
    {
        ierror = MPI_Recv(&msg, 1, MPI_INT, uniSize - 1, 101, MPI_COMM_WORLD, &status);
        printf("Using blocking communication, the message received using %d processors is: %d\n", uniSize, msg);
    }

    // Part 2 - Non-Blocking Communication

    // Initializing the request flag (?) that will inidcate when the non-blocking operation is completed.
    MPI_Request request;

    // Resetting message value to 0.
    msg = 0;

    // As above, checking if we're on process 0 and if not receiving from the previous process.
    if (myRank != 0)
    {
        // Receiving the message and waiting for the task to be complete. In this case, there is no task to do
        // before receving the message, so it's a little redundant.
        ierror = MPI_Irecv(&msg, 1, MPI_INT, myRank - 1, 101, MPI_COMM_WORLD, &request);
        // If there was work to do, it would be done here
        ierror = MPI_Wait(&request, &status);
    }

    // Incrementing message by rank value
    msg += myRank;

    // Sending the message once the subsequent processor is ready to receive.
    ierror = MPI_Isend(&msg, 1, MPI_INT, (myRank + 1) % uniSize, 101, MPI_COMM_WORLD, &request);
    // ...some work here...
    ierror = MPI_Wait(&request, &status);

    // Checking if we're back to rank 0. If we are, wait until the message comes in and then print the value received.
    if (myRank == 0)
    {
        ierror = MPI_Irecv(&msg, 1, MPI_INT, uniSize - 1, 101, MPI_COMM_WORLD, &request);
        ierror = MPI_Wait(&request, &status);
        printf("Using non-blocking communication, the message received using %d processors is: %d\n", uniSize, msg);
    }

    // Part 3 - MPI_Ireduce

    // Resetting the message again

    msg = 0;

    // Initializing a local variable for each processor to pass a value to.
    int local_msg = myRank;

    // On process 0, once all other processes are complete, compile the values given to it.
    ierror = MPI_Ireduce(&local_msg, &msg, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD, &request);
    // ...work here...
    ierror = MPI_Wait(&request, &status);

    // Printing result.
    if (myRank == 0)
    {
        printf("Using MPI_Ireduce, the message received using %d processors is: %d\n", uniSize, msg);
    }

    // Finalizing.
    ierror = MPI_Finalize();

    return 0;
}