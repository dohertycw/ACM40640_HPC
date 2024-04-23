In this folder is my code for question 1.

It performs and prints three actions:
    - Sending a message between processors, adding the rank of the processor to the message each time
      until all processors have done it. Then, the rank 0 processor prints the received message.
    - Does the same as the above, but uses non-blocking communication instead.
    - This time, each processor assigns their rank to a local variable. Then, each local variable is collected
      using the non-blocking reduction function and printed. The answer should be the same as the above.

To compile the code run:
    mpicc ring.c -o ring -lm

And to run the programme use:
    mpirun -n <number of processors desired> ring

If no number of processors are specified, MPI defaults to the number available on the user's machine. On sciprog, this is 3.