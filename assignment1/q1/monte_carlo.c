#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<omp.h>
//including the given ran2 function as a header file
#include "ran2.h"

int hits(int nThreads, int nt);

//Initialising main function
int main(int argc, char *argv[]){
	//Initialising variables
	int i, j, k, nd, nt, nThreads;
	long seed;
	float *x, *y, d;
	double pi_v;
	
	/*
	//Taking a user-provided integer number of iterations
	printf("Please choose a (large) integer number of iterations (e.g. 1000):\n");
	scanf("%d",&nt);
	//Programme needs at least 1 iteration.
	if(nt < 1){
		printf("At least try 1 iteration. Exiting. \n");
		exit(1);
	}
	*/
	
	/*
	//Taking a user-defined integer number of threads.
	//Here 10 is taken as the maximum to avoid significant slow down.
        printf("Please enter an integer number of threads (no more than 10 please!): \n");
        scanf("%d", &nThreads);
	//If user tries to input less than 1 or more than 10, programme exits.
	if(nThreads > 10 || nThreads < 1){
		printf("Please choose between an amount of threads between 1 and 10.\n");
		exit(2);
	}*/

	nt = atoi(argv[1]);
	nThreads = atoi(argv[2]);

	//Initialising nd at 0.
	nd = 0;
	
	for(i = 1; i< 20; i++){
		nd = 0;
		nd = hits(nThreads, nt);
		printf("%d\n", nd);
		pi_v = 4.0*(nd)/(nt);
		printf("With %d thread(s) pi is estimated to be: %f\n", nThreads, pi_v);
	}

	/*nd = hits(nThreads, nt);
	printf("%d\n", nd);*/

	//Declaring an OpenMP parallel construct for my for-loop.
	//i is the only private variable from thread to thread, with everything else being shared.
	//I've also used reduction() to allow each thread to add to the nd variable independently.

	/*for(i = 0; i<10; i++){
		printf("%f, %f\n", x[i],y[i]);
	}*/
	
	return 0;
}

int hits(int nThreads, int nt){
	int i, nd;
	float x, y, d;
	long seed = 2;
	long tid = omp_get_thread_num();
	nd = 0;
	#pragma omp parallel for num_threads(5) firstprivate(seed) private(i, tid) reduction(+:nd)
	for(i = 0; i < nt; i++){
		seed = seed + tid;
		x = ran2(&seed);
		y = ran2(&seed);
		#pragma omp critical
		{
		d = x*x + y*y;
		}
		/*if(i < 5) printf("%f, %f\n", x, y);*/
		if(d<=1) nd++;
	}
	return nd;
}
