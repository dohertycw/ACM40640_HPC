#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <float.h>
#include <math.h>

struct matrix
{
    int nrow;
    int ncol;
    float *matrix;
};

int getindex(struct matrix m, int i, int j);

int main(void)
{
    int i, j, k;
    struct matrix a, b, c;
    FILE *fp;

    // Read in matrix sizes

    printf("Enter size of matrix A: (format: row, column)\n");
    scanf("%d, %d", &a.nrow, &a.ncol);
    printf("Matrix A is a %dx%d matrix\n", a.nrow, a.ncol);

    printf("Enter size of matrix B: (format: row, column)\n");
    scanf("%d, %d", &b.nrow, &b.ncol);
    printf("Matrix B is a %dx%d matrix\n", b.nrow, b.ncol);

    // Check compatibility

    if (a.ncol != b.nrow)
    {
        printf("Incompatible matrices for multiplication. Exiting.\n");
        exit(1);
    }
    else
    {
        c.nrow = a.nrow;
        c.ncol = b.ncol;
    }

    // Allocating memory.

    a.matrix = (float *)malloc(a.nrow * a.ncol * sizeof(float));
    b.matrix = (float *)malloc(b.nrow * b.ncol * sizeof(float));
    c.matrix = (float *)malloc(c.nrow * c.ncol * sizeof(float));

    // Generating random arrays.

    srand(10244);

#pragma omp parallel private(i, j, k) shared(a, b, c)
    {
#pragma omp sections
        {
#pragma omp section
            for (i = 0; i < a.nrow; i++)
            {
                for (j = 0; j < a.ncol; j++)
                {
                    a.matrix[getindex(a, i, j)] = 2.0 * rand() / RAND_MAX;
                }
            }
#pragma omp section
            for (i = 0; i < b.nrow; i++)
            {
                for (j = 0; j < b.ncol; j++)
                {
                    b.matrix[getindex(b, i, j)] = 2.0 * rand() / RAND_MAX;
                }
            }
#pragma omp section
            for (i = 0; i < c.nrow; i++)
            {
                for (j = 0; j < c.ncol; j++)
                {
                    c.matrix[getindex(c, i, j)] = 2.0 * rand() / RAND_MAX;
                }
            }
        }
    }

    // Multiplying A and B
    for (i = 0; i < c.nrow; i++)
    {
        for (j = 0; j < c.ncol; j++)
        {
            for (k = 0; k < a.ncol; k++)
            {
                c.matrix[getindex(c, i, j)] += a.matrix[getindex(a, i, k)] * b.matrix[getindex(b, k, j)];
            }
        }
    }

    // writing to a file

    fp = fopen("Cmatrix.dat", "w");
    if (fp == NULL)
    {
        printf("Cannot open file!\n");
        exit(2);
    }

    for (i = 0; i < c.nrow; i++)
    {
        for (j = 0; j < c.ncol; j++)
        {
            fprintf(fp, "%f, ", c.matrix[getindex(c, i, j)]);
        }
        fprintf(fp, "\n");
    }

    // cleanup

    fclose(fp);
    free(a.matrix);
    free(b.matrix);
    free(c.matrix);

    return 0;
}

int getindex(struct matrix m, int i, int j)
{
    return m.ncol * i + j;
}