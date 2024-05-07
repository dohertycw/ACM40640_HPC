#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

// Initialising function to convert between degrees and radians.
double degtorad(double degang)
{
    return ((M_PI * degang) / 180.0);
}

// Initialising function to be integrated
double func(double x)
{
    return tan(x);
}

// Initialising trapezoidal function.
double trap(int n, double h, double *fx)
{
    double area = 0.0;
    int i;
    for (i = 0; i <= n; i++)
    {
        // If clause for a and b, the only two f(x) values multiplied by h/2
        if (i == 0 || i == n)
        {
            area += (h / 2) * fx[i];
        }
        else
        {
            area += h * fx[i];
        }
    }

    // Converting resulting area to radians.
    area = degtorad(area);

    // Returning area to main function
    return area;
}

// Constructing main function
int main(int argc, char **argv)
{
    // Initialising variables:
    int n, i, nlocal, ierror, myRank, uniSize;
    double alocal, blocal;
    double a, b, h, arealocal, area, degang, hlocal;
    double *xlocal, *fxlocal;
    double diff, tol;

    // Assigning necessary values to a and b
    a = 0;
    b = 60;

    // Initialising for-loop using (slightly above) the value for n found using the serial code to find area under tangent curve between 0 and 60.
    n = 550;

    // Initialising MPI environment and broadcasting a, b and n from rank 0
    ierror = MPI_Init(&argc, &argv);
    ierror = MPI_Comm_size(MPI_COMM_WORLD, &uniSize);
    ierror = MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
    ierror = MPI_Barrier(MPI_COMM_WORLD);
    ierror = MPI_Bcast(&a, 1, MPI_INT, 0, MPI_COMM_WORLD);
    ierror = MPI_Bcast(&b, 1, MPI_INT, 0, MPI_COMM_WORLD);
    ierror = MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    ierror = MPI_Barrier(MPI_COMM_WORLD);

    // Calculating local a, b and n
    nlocal = (n + (uniSize-(n%uniSize)))/uniSize; 
    alocal = myRank * nlocal;
    blocal = (myRank + 1) * nlocal;
    for (i = alocal; i < blocal; i++)
    {
        // Calculating hlocal
        hlocal = (blocal - alocal) / nlocal;

        // Allocating local x and f(x) arrays
        xlocal = (double *)malloc((blocal + 1) * sizeof(double));
        fxlocal = (double *)malloc((blocal + 1) * sizeof(double));

        degang = 0.0;

        // Calculate the values of x and fx arrays
        degang = hlocal * i;
        xlocal[i] = degtorad(degang);
        fxlocal[i] = func(xlocal[i]);

        // Calling trap function to find an approximated value for the area
        arealocal = trap(blocal, hlocal, fxlocal);

        // Reducing over the local area variables
        ierror = MPI_Reduce(&arealocal, &area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        // Cleanup
        free(xlocal);
        free(fxlocal);
    }

    ierror = MPI_Finalize();

    if (myRank == 0)
    {
      printf("area = %lf\n", area);
      diff = fabs(log(2.0) - (area));
      printf("The difference between log(2) and tan(x) with x = [0,60] using MPI parallelization was found to be: %.16f\n", diff);
    }


    return 0;
}

// The difference between log(2) and tan(x) with x = [0,60] using MPI parallelization was found to be: 437051976388301.3750000000000000
// Hmm... that's not right? But I can't seem to see where I'm going wrong. The code is running at least, so that's something...