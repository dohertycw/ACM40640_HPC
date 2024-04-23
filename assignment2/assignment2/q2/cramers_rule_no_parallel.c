#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// creating a loop to generate the requested matrix:

double det5(double **M);
double det4(double M[4][4]);
double det3(double M[3][3]);

int main()
{
    int i, j;
    double **M, den;
    FILE *fp;

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

    fp = fopen("matrix_initial.dat", "w");

    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 5; j++)
        {
            fprintf(fp, "%f ", M[i][j]);
        }
        fprintf(fp, "\n");
    }

    printf("%.16f \n",det5(M));

    printf("%.16f \n", -2678797333.0/88905600000.0);

    fclose(fp);
    free(M);

    return 0;
}

double det5(double **M)
{
    int i, j, k, col, row;
    double det, A[4][4];

    det = 0.0;
    for (i = 0; i < 5; i++)
    {
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
        det += M[0][i] * pow(-1.0, 1.0 * i) * det4(A);
    }
    //printf("%f\n", det);
    return det;
}

double det4(double M[4][4])
{
    int i, j, k, col, row;
    double det, A[3][3];

    det = 0.0;
    for (i = 0; i < 4; i++)
    {
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
        det += M[0][i] * pow(-1.0, 1.0 * i) * det3(A);
    }
    //printf("%f\n", det);
    return det;
}

double det3(double M[3][3])
{
    return M[0][0] * (M[1][1] * M[2][2] - M[1][2] * M[2][1]) - M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]) + M[0][2] * (M[1][0] * M[2][1] - M[1][1] * M[2][0]);
}
