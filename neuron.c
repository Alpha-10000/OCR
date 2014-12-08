#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "neuron.h"
#include "math.h"

neuron *initNeuron(int nbEntries)
{
  neuron *test = malloc(sizeof(neuron));
  test->nbEntries = nbEntries;
  test->entries = malloc(test->nbEntries * sizeof(double));
  test->weight = malloc(test->nbEntries * sizeof(double));
  for(int i = 0; i < nbEntries; i++)
  {
    test->weight[i] = (rand()/(double)RAND_MAX)-0.5;
    test->entries[i] = 1;
  }
  test->output = 0;
  test->delta = 0;
  test->epsilon = 1;
  return test;
}

void calculateOutput(neuron *neuron)
{
  double sum = 0;
  for(int i = 0; i < neuron->nbEntries; i++)
  {
    sum += neuron->entries[i] * neuron->weight[i];
  }
  //This is where we apply the function
  //printf("resulting sum: %f\n", sum);
  neuron->output = (1/(1+exp(-sum)));
  //printf("sigmoid=%f\n", neuron->output);
  //neuron->output = neuron->output >= 0.5 ? 1 : 0;
  //printf("resulting output: %f\n", neuron->output);
}

void freeMemory(neuron *entry)
{
  free(entry->weight);
  free(entry->entries);
  free(entry);
}
