#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<float.h>
#include<math.h>

//Comments explaining things not explained here can be found in matmult_no_omp.c

double dot(double **A, double **B, int n, int q, int p);

int main(void){
	int n, m, p, q, i, j, h, nSamples;
	double **A, **B, **C,*times, denom, num, start, end, tmin, tmax;
	srand(5);
	
	//Enter array size
	printf("Please enter array dimensions (n x m) for array A: \n");
	scanf("%d", &n);
	scanf("%d", &m);
	
        printf("Please enter array dimensions (p x q) for array B: \n");
        scanf("%d", &p);
        scanf("%d", &q);

	//Enter sample size
	printf("Please enter a sample size: \n");
	scanf("%d", &nSamples);
	printf("Programme will run %d times. \n", nSamples);

	if (n < 1 || m < 1 || p<1 || q<1 ){
		printf("Arrays must be at least 1x1. \n");
		exit(1);
	}
	if (n>50 || m>50 || p>50 || q>50){
		printf("Arrays must be at most 50x50. \n");
		exit(2);
	}
	if (m != p){
		printf("Unable to multiply %d x %d matrix by %d x %d matrix. \n# of rows in A must equal # of columns in B. \n", n, m, p, q);
		exit(3);
	}
	if (nSamples < 1){
		printf("Insufficient sample size. \n");
		exit(4);
	}
	
	printf("\n");

	times = (double *)malloc(nSamples*sizeof(double));
	tmin = DBL_MAX;
	tmax = 0.0;
	A = (double **)malloc(n*sizeof(double));
	B = (double **)malloc(p*sizeof(double));

	//initialising parallel sections within which the matrices will be generated
	#pragma omp parallel sections
	{
	printf("A: \n");
	#pragma omp section
	{
	for(i = 0; i < n; i++) {
		A[i] = (double *)malloc(m*sizeof(double));
		for(j = 0; j<m; j++){
			num = rand();
			denom = rand();
			A[i][j] = num/denom;
			printf("%f ", A[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	}
	#pragma omp section
	{
	printf("B: \n");
	for(i = 0; i<p; i++) {
		B[i] = (double *)malloc(q*sizeof(double));
		for(j = 0; j<q; j++) {
			num = rand();
			denom = rand();
			B[i][j] = num/denom;
			printf("%f ", B[i][j]);
		}
		printf("\n");
	}

	printf("\n");
	}
	}
	C = (double **)malloc(n*sizeof(double));
	for(i = 0; i < n; i++){
		C[i] = (double *)malloc(q*sizeof(double));
	}


	//Calculating C over multiple iterations.
	//The start and end times for the task are recorded, with the difference between the two given to the times array
	for(h = 0; h < nSamples; h++){
		start = omp_get_wtime();
		for(i = 0; i < n; i++){
			for(j = 0; j < q; j++){
				C[i][j] = dot(A,B,i,j,p);
			}
		}
		end = omp_get_wtime();
		times[h] = end - start;
	}
	//Calculates average time, although I don't print it right now
	double timeAvg = 0.0;
	for(i = 0; i<nSamples; ++i){
		timeAvg+=times[i];
	}
	timeAvg/=nSamples;

	printf(" #Loop|    C Dimensions|        Time(s)|\n");
	for(i = 0; i<nSamples; i++){
		printf("%6d %12d x %d %16.8f\n", i+1, n, q, times[i]);
	}
	
	//Printing C
	printf("\nA x B = C:\n");
	for(i = 0; i < n; i++){
		for(j = 0; j < q; j++){
			printf("%f ", C[i][j]);
		}
		printf("\n");
	}

	free(times);	
	free(A);
	free(B);
	free(C);
	return 0;
}

double dot(double **A, double **B, int n, int q, int p){
	int i, j, k;
	double prod = 0.0;
	for(k = 0; k < p; k++){
		prod += A[n][k]*B[k][q];
	}
	return prod;
}
	
