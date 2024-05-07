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

// Initialising trapezoidal function using OpenMP.
double trap(int n, double h, double *fx)
{
  double area = 0.0;
  int i, tid, nthreads;
  // Initialising parallel region
#pragma omp parallel num_threads(10) shared(n, area, fx, h, nthreads) private(tid)
  {
    // Printing number of threads and thread ids as requested.
    tid = omp_get_thread_num();
    nthreads = omp_get_num_threads();
    printf("hello from thread %d out of %d\n", tid, nthreads);
#pragma omp for private(i)
    for (i = 0; i <= n; i++)
    {
      if (i == 0 || i == n)
      {
        area += (h / 2) * fx[i];
      }
      else
      {
        area += h * fx[i];
      }
    }
  }

  area = degtorad(area);

  return area;
}

// Constructing main function
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
  n = 550;
  for(i = 0; i<=n; i++)
  {
    // Calculating h for each instance of the loop.
    h = (b - a) / n;

    // Dynamically allocating memory for x and f(x) arrays
    x = (double *)malloc((n+1) * sizeof(double));
    fx = (double *)malloc((n+1) * sizeof(double));

    degang = 0.0;

    // Calculating values in x and f(x) arrays.
    degang = h*i;
    x[i] = degtorad(degang);
    fx[i] = func(x[i]);

    // Calculating approximate area under curve
    area = trap(n, h, fx);

    // Checking difference between actual value and calculated value
    diff = fabs(log(2.0)-area);
    printf("The difference between the actual value and trapezoidal rule value using OMP parallelization with 10 threads is : %.16f\n", diff);

    // Cleanup
    free(x);
    free(fx);
  }

  return 0;
}


// The difference between the actual value and trapezoidal rule value using OMP parallelization with 3 threads is : 0.0000208450536415
// The difference between the actual value and trapezoidal rule value using OMP parallelization with 5 threads is : 0.0000208450536417
// The difference between the actual value and trapezoidal rule value using OMP parallelization with 10 threads is : 0.0000208450536411

// As we can see, the difference each time is marginal, indicating we would need more runs for this parallelization to be effective.