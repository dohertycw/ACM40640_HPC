#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ran2.h"

//Initialising main function

int main(void){
	//Initialising variables and seed value. 
	int i, nd, nt;
	float x,y;
	double d;
	long seed = 2;

	//Initialising nd and nt value. nt was chosen as 1000 arbitrarily.
	nt = 1000;
	nd = 0;

	//Initialising for loop to generate range of random values for x and y.
	
	for(i = 0;i < nt; i++){
		x = ran2(&seed);
		y = ran2(&seed);
		//Calculating distance from the origin for randomly generated points.
		d = sqrt(x*x + y*y);
		if(d <= 1){
			nd++;
		}
	}

	//Calculating and printing pi estimate.

	printf("Our estimate for pi using the Monte Carlo method is %f\n", 4.0*(nd)/(nt));
	return 0;
}
