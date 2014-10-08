#include <stdlib.h>
#include <stdio.h>

typedef struct neuron neuron;

struct neuron
{
	int nbEntries;
	double *weight; 
	double output;
	double errorValue;
};

void Initialize(neuron *test)
{
	test->nbEntries = 4;
	printf("%d\n", test->nbEntries);
	test->weight = malloc(test->nbEntries * sizeof(double));
	test->errorValue = 5;
}

void FreeMemory(neuron *test)
{
	free(test->weight);
	printf("The memory has been freed!\n");	
}
