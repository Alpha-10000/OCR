#include <stdlib.h>
#include <stdio.h>

typedef struct neuron neuron;

struct neuron
{
	int nbEntries;
	int *entries;
	double *weight;
	int output;
	double bias;
	double epsilon;
};

void CalculateOutput(neuron *neuron);

neuron *Initialize(int nb_Entries)
{
	neuron *test = malloc(sizeof(neuron));

	test->nbEntries = nb_Entries;

	test->entries = malloc(test->nbEntries * sizeof(int));
	test->weight = malloc(test->nbEntries * sizeof(double));
	for(int i = 0; i < nb_Entries; i++)
	{
		test->weight[i] = 1;
		test->entries[i] = 1;
	}
	test->output = 0;
	test->bias = 0;
	test->weight[0] = 1;
	test->weight[1] = -1;
	test->epsilon = 1;
	return test;
}

void Learn(neuron *neuron)
{
	int testVector[2];

	for(int i = 0; i < 10; i++)
	{

		testVector[0] = 0;
		testVector[1] = 0;

		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->entries[i] = testVector[i];	
		}
		CalculateOutput(neuron);
		neuron->bias = neuron->bias + neuron->epsilon*(0-neuron->output)*1;
		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->weight[i] = neuron->weight[i] 
				+ neuron->epsilon*(0-neuron->output)*neuron->entries[i];
		}

		testVector[0] = 1;
		testVector[1] = 0;

		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->entries[i] = testVector[i];	
		}
		CalculateOutput(neuron);
		neuron->bias = neuron->bias + neuron->epsilon*(0-neuron->output)*1;
		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->weight[i] = neuron->weight[i] 
				+ neuron->epsilon*(0-neuron->output)*neuron->entries[i];
		}

		testVector[0] = 0;
		testVector[1] = 1;

		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->entries[i] = testVector[i];	
		}
		CalculateOutput(neuron);
		neuron->bias = neuron->bias + neuron->epsilon*(0-neuron->output)*1;
		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->weight[i] = neuron->weight[i] 
				+ neuron->epsilon*(0-neuron->output)*neuron->entries[i];
		}

		testVector[0] = 1;
		testVector[1] = 1;

		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->entries[i] = testVector[i];	
		}
		CalculateOutput(neuron);
		neuron->bias = neuron->bias + neuron->epsilon*(1-neuron->output)*1;
		for(int i = 0; i < neuron->nbEntries; i++)
		{
			neuron->weight[i] = neuron->weight[i] 
				+ neuron->epsilon*(1-neuron->output)*neuron->entries[i];
		}
	}
	printf("Done learning\n");

}

void CalculateOutput(neuron *neuron)
{
	double sum = 1*neuron->bias;
	for(int i = 0; i < neuron->nbEntries; i++)
	{
		sum += neuron->entries[i] * neuron->weight[i];
	}
	neuron->output = sum > 0 ? 1 : 0;
}

void FreeMemory(neuron *entry)
{
	free(entry->weight);
	free(entry->entries);
	free(entry);
	printf("The memory has been freed!\n");	
}
