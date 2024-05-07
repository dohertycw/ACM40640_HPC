#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
// Note: Apply MPI parallelisation in the main function.
int main(int argc, char **argv)
{
    // Initialising variables:
    int n, i, nlocal, ierror, myRank, uniSize;
    double a, b, h, arealocal, area, degang;
    double *xlocal, *fxlocal;
    double diff, tol;

    // Assigning necessary values to a and b
    a = 0;
    b = 60;

    // Initialising values for diff and tol.
    diff = 1.1 * pow(10.0, -(6.0));
    tol = 1.0 * pow(10.0, -(6.0));

    // Initialising for-loop using (slightly above) the value for n found using the serial code to find area under tangent curve between 0 and 60.
    n = 550;

    nlocal = (n + (uniSize - (n % uniSize))) / (uniSize) for (i = myRank * nlocal; i < (myRank + 1) * nlocal; i++)
    {
        // Note: The interval [a,b] is split up into n subintervals of width h
        // TODO:calculate the value of h
        h = (b - a) / nlocal;

        // TODO: Allocate x and fx vectors where x=[a x1 ... b]
        // and fx =[f(a) f(x1) ... f(b)] and a=x0, b=xn
        // Note: the value of the pts and the value of the function at these pts
        // are stored as arrays
        xlocal = (double *)malloc((nlocal + 1) * sizeof(double));
        fxlocal = (double *)malloc((nlocal + 1) * sizeof(double));

        degang = 0.0;

        // TODO: Calculate the values of x and fx arrays
        // Note: The values of xi should be in radians
        for (i = 0; i <= n; i++)
        {
            degang = h * i;
            xlocal[i] = degtorad(degang);
            fxlocal[i] = func(xlocal[i]);
            // printf("The tan of %f radians is %f\n", x[i], fx[i]);
        }

        // TODO: Call trap function to find an approximated value for the area
        arealocal = trap(nlocal, hlocal, fxlocal);

        ierror = MPI_Reduce(&arealocal, &area, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

        if (myRank == 0)
        {
            diff = fabs(log(2.0) - area);
            printf("The difference between log(2) and tan(x) with x = [0,60] using OpenMP parallelisation with 10 threads was found to be: %.16f\n", diff);
        }
        // TODO: Free the memory space for x and fx
        free(xlocal);
        free(fxlocal);
    }

    return 0;
}