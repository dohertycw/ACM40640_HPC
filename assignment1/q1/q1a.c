#include<stdlib.h>
#include<stdio.h>
#include "ran2.h"

//Initialising main function

int main(void){
	//Initialising variables and seed value. 
	int i;
	float x,y;
	long seed = 2;

	printf("      x:|      y: \n");

	//Initialising for loop to generate range of random values for x and y. The number of iterations was chosen arbitrarily.
	
	for(i = 0;i < 10; i++){
		x = ran2(&seed);
		y = ran2(&seed);
		printf("%3f  %3f\n", x, y);
	}
	return 0;
}
