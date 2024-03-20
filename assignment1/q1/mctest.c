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

    #pragma omp parallel
    {
	for(i = 0;i < nt; i++){
            seed = i;
			x = ran2(&seed);
			y = ran2(&seed);
			// if(i<10) printf("%f, %f\n", x,y);
			//Calculating distance from the origin for randomly generated points.
			d = sqrt(x*x + y*y);
			if(d <= 1){
				//Using atomic to sum my nd value.
				nd++;
		}
	}
    }

	//Calculating and printing pi estimate.
	printf("Our estimate for pi using the Monte Carlo method with 2 threads is %f\n", 4.0*(nd)/(nt));
	return 0;
}
	