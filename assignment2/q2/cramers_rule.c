#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Initializing functions to calculate the determinants of the minors

double det4(double M[4][4]);
double det3(double M[3][3]);

// Initializing main function

int main(int argc, char **argv)
{
    // Declaring variables
    int i, j, k, col, row, ierror, uniSize, myRank;
    double **M, den, det, local_det, A[4][4];

    // Declaring a file, so that initial matrix can be seen
    FILE *fp;

    // Allocating initial matrix
    M = (double **)malloc(5 * sizeof(double *));

    for (i = 0; i < 5; i++)
    {
        M[i] = (double *)malloc(5 * sizeof(double));
    }

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            den = (i + j + 1);
            if (i == j)
            {
                M[i][j] = 1 / den;
            }
            else
            {
                M[i][j] = -1 / den;
            }
        }
    }

    // Printing initial matrix to file 
    fp = fopen("matrix_initial.dat", "w");

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            fprintf(fp, "%f ", M[i][j]);
        }
        fprintf(fp, "\n");
    }

    // Initializing MPI instance
    ierror = MPI_Init(&argc, &argv);

    // Getting total number of processors and their respective ranks
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    // Conditional to abort programme if the total number of processors declared isn't 5, as was requested in the question
    if (uniSize != 5)
    {
        if (myRank == 0)
        {
            printf("!! Question requested the use of 5 processes !! \n Please run mpirun -n 5 cramers to run.\n");
        }
        ierror = MPI_Finalize();
        ierror = MPI_Abort(MPI_COMM_WORLD, 10);
    }

    // Initializing determinant at 0 to begin
    det = 0.0;

    // Calculating determinant of minor relative to the processor we're on. There's probably a more elegant way to do this, but this works.
    for (i = myRank; i < (myRank + 1); i++)
    {
        // Generating submatrix
        row = 0;
        col = 0;
        for (j = 0; j < 4; j++)
        {
            for (k = 0; k < 5; k++)
            {
                if (k == i)
                {
                    continue;
                }
                else
                {
                    A[row][col] = M[j + 1][k];
                    col++;
                    if (col == 4)
                    {
                        row++;
                        col = 0;
                    }
                }
            }
        }
        // Passing submatrix to our 4x4 matrix determinant calculation function, multiplying it by it's corresponding value in the 5x5 matrix
        // and passing it to local_det variable to be passed to our Reduce function.
        local_det = M[0][i] * pow(-1.0, 1.0 * i) * det4(A);
    }

    // Reducing across all processors' local determinants.
    ierror = MPI_Reduce(&local_det, &det, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    // Concluding MPI environment
    ierror = MPI_Finalize();

    // Printing result and comparing with given answer.
    if (myRank == 0)
    {
        printf("Result computed using MPI parallelization: %.16f\n", det);

        printf("Given answer: %.16f \n", -2678797333.0 / 88905600000.0);

	    printf("Comparing answers: %d \n", det - (-2678797333.0 / 88905600000.0));
    }

    // Cleanup
    fclose(fp);
    free(M);

    return 0;
}

// Building function to calculate determinant of a 4x4 matrix
double det4(double M[4][4])
{
    // Declaring variables
    int i, j, k, col, row;
    double det, A[3][3];

    det = 0.0;
    for (i = 0; i < 4; i++)
    {
        // Constructing necessary 3x3 submatrices
        row = 0;
        col = 0;
        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 4; k++)
            {
                if (k == i)
                {
                    continue;
                }
                else
                {
                    A[row][col] = M[j + 1][k];
                    col++;
                    if (col == 3)
                    {
                        row++;
                        col = 0;
                    }
                }
            }
        }
        // Calculating determinant
        det += M[0][i] * pow(-1.0, 1.0 * i) * det3(A);
    }

    // Returning value
    return det;
}

// Function to calculate the determinant of a 3x3 matrix. Again, there's probably cleaner, nicer looking ways to do this but this works.
double det3(double M[3][3])
{
    return M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) - M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]) + M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]);
}
