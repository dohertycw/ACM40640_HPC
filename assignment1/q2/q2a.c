#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

double wnew(double **w, int i, int j);

int main(void)
{
	int i, j, k;
	double **w, **temp, diff, max_diff;
	FILE *fp1, *fp2;

	w = (double **)malloc(100 * sizeof(double *));
	temp = (double **)malloc(100 * sizeof(double *));
	for (i = 0; i < 100; i++)
	{
		w[i] = (double *)malloc(100 * sizeof(double));
		temp[i] = (double *)malloc(100 * sizeof(double));
	}

#pragma omp parallel private(i, j) shared(w)
	{
#pragma omp for
		for (i = 0; i < 100; i++)
		{
			for (j = 0; j < 100; j++)
			{
				if (j == 0 || j == 99 || i == 99)
				{
					w[i][j] = 100.0;
					temp[i][j] = w[i][j];
				}
				else if (i == 0 && 1 < j < 98)
				{
					w[i][j] = 0.0;
					temp[i][j] = w[i][j];
				}
				else
				{
					w[i][j] = 75.0;
					temp[i][j] = w[i][j];
				}
			}
		}
	}

	fp1 = fopen("matrix_initial.dat", "w");

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			fprintf(fp1, "%f ", w[i][j]);
		}
		fprintf(fp1, "\n");
	}
	max_diff = 1.1 * pow(10.0, -4.0);

	while (max_diff > pow(10.0, -4.0))
	{
		diff = 0.0;
		max_diff = 0.0;
#pragma omp parallel shared(w, temp, diff) private(i, j) reduction(max : max_diff)
		{
#pragma omp for
			for (i = 1; i < 99; i++)
			{
				for (j = 1; j < 99; j++)
				{
					temp[i][j] = wnew(w, i, j);
					diff = fabs(w[i][j] - temp[i][j]);
					w[i][j] = temp[i][j];
					if (diff > max_diff)
					{
						max_diff = diff;
					}
				}
			}
		}
	}

	fp2 = fopen("matrix_updated.dat", "w");

	for (i = 0; i < 100; i++)
	{
		for (j = 0; j < 100; j++)
		{
			fprintf(fp2, "%f ", w[i][j]);
		}
		fprintf(fp2, "\n");
	}

	fclose(fp1);
	fclose(fp2);
	free(w);
	free(temp);

	return 0;
}

double wnew(double **w, int i, int j)
{
	return (w[i + 1][j] + w[i - 1][j] + w[i][j + 1] + w[i][j - 1]) / 4.0;
}
