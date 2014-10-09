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

void calculateOutput(neuron *neuron);

neuron *initialize(int nb_Entries)
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

void learn(neuron *neuron, int inputs[][2], int expected[], int size)
{
	for(int i = 0; i < 10; i++) //Test Number
	{
		for(int j = 0; j < size; j++) //We do each different test
		{
			for(int k = 0; k < neuron->nbEntries; k++)
			{
				neuron->entries[k] = inputs[j][k];
			}
			calculateOutput(neuron);
			neuron->bias = neuron->bias 
				+ neuron->epsilon*(expected[j]-neuron->output)*1;
			for(int l = 0; l < neuron->nbEntries; l++)
			{
				neuron->weight[l] = neuron->weight[l] 
				+ neuron->epsilon*(expected[j]-neuron->output)*neuron->entries[l];
			}
		}
	}
	printf("Done learning\n");

}

void calculateOutput(neuron *neuron)
{
	double sum = 1*neuron->bias;
	for(int i = 0; i < neuron->nbEntries; i++)
	{
		sum += neuron->entries[i] * neuron->weight[i];
	}
	neuron->output = sum > 0 ? 1 : 0;
}

void freeMemory(neuron *entry)
{
	free(entry->weight);
	free(entry->entries);
	free(entry);
	printf("The memory has been freed!\n");	
}
