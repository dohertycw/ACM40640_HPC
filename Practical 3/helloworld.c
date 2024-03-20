#include<stdio.h>
#include<omp.h>
int main(void){
	int tid, nthreads;
	#pragma omp parallel private(tid), shared(nthreads)
	{
	tid = omp_get_thread_num();
	nthreads = omp_get_num_threads();
	printf("Hello from thread %d out of %d\n", tid, nthreads);
	}
	return 0;
}
