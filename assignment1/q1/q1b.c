#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "ran2.h"

//Initialising main function

int main(void){
	//Initialising variables and seed value. 
	int i;
	float x,y;
	double d;
	long seed = 2;

	//Initialising for loop to generate range of random values for x and y. The number of iterations was chosen arbitrarily.
	
	for(i = 0;i < 10; i++){
		x = ran2(&seed);
		y = ran2(&seed);
		//Calculating distance from the origin for randomly generated points.
		d = sqrt(x*x + y*y);
		printf("(%f,  %f)\n", x, y);
		//Printing distance from origin.
		printf("Distance from origin = %f\n", d);
	}
	return 0;
}
