#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ran2.h"

//Initialising main function

int hits(int nt);

int main(void){
	//Initialising variables and seed value. 
	int i, nd, nt;
	float x,y;
	double d;
	/*long seed = 2;*/

	//Initialising nd and nt value. nt was chosen as 1000 arbitrarily.
	nt = 1000;
	nd = 0;

	/*printf("Testing for thread safety.\n");
	#pragma omp parallel for default(shared) private(i) reduction(+:nd)
	for(i = 0; i<10; i++){
		nd = 0;
		printf("Run %d\n", i);
		nd = hits(nt);
		printf("%d\n", nd);
	}*/
	for(i = 0; i<5; i++){
		nd = hits(nt);
		printf("%d\n", nd);
	}

		
	//Calculating and printing pi estimate.

	printf("Our estimate for pi using the Monte Carlo method is %f\n", 4.0*(nd)/(nt));
	return 0;
}

int hits(int nt){
	int i, nd;
	long seed;
	float x, y, d;
	nd = 0;
        #pragma omp parallel for num_threads(5) default(shared) private(i, seed, x, y, d) reduction(+:nd)
        for(i = 0;i < nt; i++){
		seed = omp_get_thread_num();
                x = ran2(&seed);
                y = ran2(&seed);
                d = x*x + y*y;
                /*if(i < 5) printf("(%f, %f)\n",x,y);*/
                if(d <= 1){
                        nd++;
		}
        }
	return nd;
}
