#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
int main(int argc, char* argv[]){
	int i, j;
	long length, nSamples;
	double *a, *b, *c, *d;
	double start, end, val;

	if(argc!=3) {
		printf("Incorrect input. \n");
		printf("Please input: %s <array length> <nSample> \n",argv[0]);
		return -1;
	}

	length = atoi(argv[1]);
	nSamples = atoi(argv[2]);
	
	a = (double*)malloc(length*sizeof(double));
	b = (double*)malloc(length*sizeof(double));
	printf("Vector a of length %d \n", length);
	for (i = 0; i<length; i++){
		val = rand()%10;
		a[i] = 1/val;
		printf("%lf \n", a[i]);
	}
	printf("\n");
	printf("Vector b of length %d \n", length);
	for (i = 0; i<length; i++){
		val = rand() % 10;
		b[i] = 1/val;
		printf("%lf \n", b[i]);
	}
	printf("\n");
	c = (double*)malloc(length*sizeof(double));
	start = omp_get_wtime();
	printf("a + b = c:\n");
	for(j =0; j<nSamples; j++){
		/*printf("Run # %d \n", j+1);*/
		for (i = 0; i<length; i++){
			c[i] = a[i] + b[i];
		}
	}
	end = omp_get_wtime();

	printf("Average execution time = %lfsec.\n", (end-start)/nSamples);

	d = (double*)malloc(length*sizeof(double));
	for (i = 0; i<length; i++){
		d[i] = 0.0;
	}

	start = omp_get_wtime();
	#pragma omp parallel for schedule(static, length)  shared(a,b,d) private(i,j)
		for(i = 0; i<nSamples; i++) {
			for (j = 0; j<length; j++){
				d[j] = a[j] + b[j];
			}
		}
        end = omp_get_wtime();

        printf("Average execution time = %lfsec.\n", (end-start)/nSamples);


	free(a);
	free(b);
	free(c);
	free(d);
	return 0;
}
