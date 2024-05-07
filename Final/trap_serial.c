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
int main()
{
  // Initialising variables:
  int n, i;
  double a, b, h, area, degang;
  double *x, *fx;
  double diff, tol;

  // Assigning necessary values to a and b
  a = 0;
  b = 60;

  // Initialising values for diff and tol.
  diff = 1.1 * pow(10.0,-(6.0));
  tol = 1.0 * pow(10.0, -(6.0));

  // Initialising and completing while-loop to find area under tangent curve between 0 and 60. Starting n at 2. 
  n = 2;
  while (diff > tol)
  {
    // Calculating h for each instance of the loop.
    h = (b - a) / n;

    // Dynamically allocating memory for x and f(x) arrays
    x = (double *)malloc((n+1) * sizeof(double));
    fx = (double *)malloc((n+1) * sizeof(double));

    degang = 0.0;

    // Calculating values in x and f(x) arrays.
    for (i = 0; i <= n; i++)
    {
      degang = h*i;
      x[i] = degtorad(degang);
      fx[i] = func(x[i]);
      // printf("The tan of %f radians is %f\n", x[i], fx[i]);
    }

    // Calculating approximate area under curve
    area = trap(n, h, fx);

    // Checking difference between actual value and calculated value
    diff = fabs(log(2.0)-area);
    printf("%.16f\n", diff);

    // Updating and printing number of intervals
    n += 1;
    printf("Increasing number of intervals to: %d\n", n);

    // Cleanup
    free(x);
    free(fx);
  }

  return 0;
}

