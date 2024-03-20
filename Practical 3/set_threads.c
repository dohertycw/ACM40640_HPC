#include<stdio.h>
#include<omp.h>
int main(void){
	int tid, nthreads;
//Here I set the number of threads to be 5 using num_threads().
#pragma omp parallel num_threads(5) private(tid), shared(nthreads)
	{
		nthreads = omp_get_num_threads();
		tid = omp_get_thread_num();
		printf("Hello from thread %d out of %d.\n", tid, nthreads);
		//This barrier environment prevents the master thread from printing the total number of threads until after the work is done.
		#pragma omp barrier
		#pragma omp master
			printf("There are %d threads.\n", nthreads);
	}

#pragma omp parallel num_threads(10) private(tid), shared(nthreads)
        {
                nthreads = omp_get_num_threads();
                tid = omp_get_thread_num();
                printf("Hello from thread %d out of %d.\n", tid, nthreads);
                #pragma omp barrier
                #pragma omp master
                        printf("There are %d threads.\n", nthreads);
        }

#pragma omp parallel num_threads(20) private(tid), shared(nthreads)
        {
                nthreads = omp_get_num_threads();
                tid = omp_get_thread_num();
                printf("Hello from thread %d out of %d.\n", tid, nthreads);
                #pragma omp barrier
                #pragma omp master
                        printf("There are %d threads.\n", nthreads);
        }
	return 0;
}                                                      
