#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<omp.h>
#include "ran2.h"

//Initialising main function
int main(void){
	//Initialising variables and seed value. 
	int i,j, nd, nt;
	float x,y;
	double d;
	long seed;

	//Initialising nd and nt value. nt was chosen as 100000 arbitrarily.
	nt = 100000;
	nd = 0;

	//Initialising omp parallel region.
	#pragma omp parallel num_threads(5) private(i,x,y,d) shared(nt,nd)
	{
		//Initialising seed, determined by the working thread to ensure thread safety
		seed = omp_get_thread_num();
		//Initialising omp for-loop region. Seed is specifically set to be private here in order to preserve thread safety.
		//If seed is set private in the initial omp parallel declaration, thread safety is lost.
		#pragma omp for private(seed)
		for(i = 0;i < nt; i++){
			x = ran2(&seed);
			y = ran2(&seed);
			if(i<10) printf("%f, %f\n", x,y);
			//Calculating distance from the origin for randomly generated points.
			d = sqrt(x*x + y*y);
			if(d <= 1.0){
				//Using atomic to sum my nd value.
				#pragma omp atomic
				nd++;
			}
		}
	}

	//Calculating and printing pi estimate.
	printf("Our estimate for pi using the Monte Carlo method is %f\n", 4.0*(nd)/(nt));
	return 0;
}
	

