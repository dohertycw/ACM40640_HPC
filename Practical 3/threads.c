#include<stdio.h>
#include<omp.h>
int main(void){
	//Intiialising necessary variables.
	int tid, nthreads;
	//Initialising OpenMP parallel region with tid being private to each thread and nthreads being shared throughout the region.
#pragma omp parallel private(tid), shared(nthreads)
	{
		//Gets the ID# of the thread it's currently in.
		tid = omp_get_thread_num();
		//Gets the number of total threads (default is 2).
		nthreads = omp_get_num_threads();
		//Printing "Hello from <current thread> out of <total threads>.
		printf("Hello from thread %d out of %d\n", tid, nthreads);
		//Master thread prints how many threads total there are.
		#pragma omp master
			printf("There are %d threads.\n", nthreads);
	}
	return 0;
}
	
