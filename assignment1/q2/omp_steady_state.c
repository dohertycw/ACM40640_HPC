#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

// Intiliasing wnew function to generate wnew.
double wnew(double **w, int i, int j);

// Initialising main function.
int main(void)
{
	// Initialising variables.
	int i, j;
	double **w, **temp, diff, max_diff;
	FILE *fp1, *fp2;

	// Allocating heap space for w and temp, the intermediate storage matrix between updates of w.
	w = (double **)malloc(100 * sizeof(double *));
	temp = (double **)malloc(100 * sizeof(double *));
	for (i = 0; i < 100; i++)
	{
		w[i] = (double *)malloc(100 * sizeof(double));
		temp[i] = (double *)malloc(100 * sizeof(double));
	}

// Parallelization of the initialisation of w. Temp is also intialised here, but initially just as a copy of w.
#pragma omp parallel num_threads(1) private(i, j) shared(w, temp)
	{
#pragma omp for
		for (i = 0; i < 100; i++)
		{
			for (j = 0; j < 100; j++)
			{
				// Logic statement to make the last row and first and last columns all be 100.0
				if (j == 0 || j == 99 || i == 99)
				{
					w[i][j] = 100.0;
					temp[i][j] = w[i][j];
				}
				// Logic statement to make the first row between 2 and 99 be 0.0
				else if (i == 0 && 1 < j < 98)
				{
					w[i][j] = 0.0;
					temp[i][j] = w[i][j];
				}
				// Setting all other elements to 75.0
				else
				{
					w[i][j] = 75.0;
					temp[i][j] = w[i][j];
				}
			}
		}
	}

	// Writing initial matrix to a document, just to see it.
	fp1 = fopen("matrix_initial.dat", "w");

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			fprintf(fp1, "%f ", w[i][j]);
		}
		fprintf(fp1, "\n");
	}

	// Initialising a max difference variable. This is to enable our while loop in the first place.
	max_diff = 1.1 * pow(10.0, -4.0);

	// Initialising while loop. It will stop once the maximum difference between w and temp(technically wnew, but just before w is updated) is at a maximu of 10^-4.
	while (max_diff > pow(10.0, -4.0))
	{
		diff = 0.0;
		max_diff = 0.0;
// Initialising parallel region to update w in parallel and check for the maximum difference.
#pragma omp parallel num_threads(1) shared(w, temp, diff) private(i, j) reduction(max : max_diff)
		{
#pragma omp for
			// For-loop to iterate over the (2:99,2:99) region of w.
			for (i = 1; i < 99; i++)
			{
				for (j = 1; j < 99; j++)
				{
					temp[i][j] = wnew(w, i, j);
					// Checking the difference between w and wnew.
					diff = fabs(w[i][j] - temp[i][j]);
					// Logic statement to check the difference between w and wnew. This will repeatedly update each iteration and if at the end of all iterations
					// max_diff is still greater than 10^-4, the loop begins again.
					if (diff > max_diff)
					{
						max_diff = diff;
					}
					// If the loop is still going, update w values.
					w[i][j] = temp[i][j];
				}
			}
		}
	}

	// Writing final matrix to a file for ease of viewing.
	fp2 = fopen("matrix_steady.dat", "w");

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			fprintf(fp2, "%f ", w[i][j]);
		}
		fprintf(fp2, "\n");
	}

	// Clean-up
	fclose(fp1);
	fclose(fp2);
	free(w);
	free(temp);

	return 0;
}

// Function to update the w values.
double wnew(double **w, int i, int j)
{
	return (w[i + 1][j] + w[i - 1][j] + w[i][j + 1] + w[i][j - 1]) / 4.0;
}

// I found that the serial runtime of the code was 0.8888595337048173s, while the runtimes of 1, 2 and 4 threads
// was 1.2000802932307124s, 50.7380839595571160s and 1.1462871897965670s respectively. Interestingly, when ran on my own
// machine I found times of 0.3145452960000057s for the sequential code and 0.3164277640000819s, 0.4131224889999885s and
// 0.4533311720000484s for 1, 2 and 4 threads. This would lead me to believe that this programme probably doesn't see enough
// iterations for parallelism to be beneficial.

// The absolute speedup is 0.7406667193175352, 0.01751858691418697 and 0.77542481641321 for 1, 2 and 4 threads respectively. This makes
// sense given that I didn't see any speedup thanks to the parallelism. The absolute efficiencies are 0.7406667193175352, 0.00875929345709348 and 
// 0.1938562041033025 for 1, 2 and 4 respectively again. Again, this makes sense given that running the code in serial ran faster than the code in parallel.

// The relative speedup of 2 and 4 threads was 0.0236524558985571 and 1.046928120555628 respectively. While it makes sense that using 4 threads decreased runtime,
// it's very unusual that using 2 threads would increase the runtime to such an extent, which I can only assume is a sciprog error.
// The relative efficiencies of 2 and 4 threads was 0.01182622794927855 and 0.261732030138907. Obviously we have our issue using 2 threads,
// but it is interesting to see that overall while using 4 threads has sped us up marginally, it's overall less efficient than simply using one thread.