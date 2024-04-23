In this folder is my code for question 2.

The code generates a 5x5 matrix, which we're requested to get the determinant of. To do so, I've defined two functions to calculate
the determinants of the 4x4 and 3x3 submatrices necessary for this calculation. Then, Cramer's rule is carried out using 5 processors,
each calculating one of the necessary minors respective to it's rank and storing it in a local variable. These variables are then
summed using MPI reduce on the rank 0 processor and printed. The answer is then compared with the given answer.

To compile the code run:
    mpicc cramers_rule.c -o cramers -lm

And to run the programme use:
    mpirun -n 5 cramers

This programme will not run without specifying 5 processors. 
