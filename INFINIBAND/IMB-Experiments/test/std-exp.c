#include <stdio.h>
#include <math.h>
double sqroot(double square)
{
	double root=square/3;
	int i;
	if (square <= 0) return 0;
	for (i=0; i<32; i++)
		root = (root + square / root) / 2;
	return root;
}

int main(){

  double std_mean = 0;
  double std_real = 0;
  double std_ele = 0;

  int i;
  int std_array[] = {155,69,490,71,399,143,429,97,389,441};

	//checking whether the results are identical
	for(i=0;i<10;i++){
		std_mean += std_array[i];
  	}

	std_mean /=10;
	std_mean = std_mean; // what?


	for(i=0;i<10;i++){
		std_ele = std_array[i];
		std_ele = std_ele-std_mean;
		std_ele = std_ele * std_ele;
		std_real+= std_ele;
	}

	std_real/=10;
	std_real = sqroot(std_real);

  printf("std-mean: %f, std-real %f\n", std_mean, std_real);

  return 0;
}
