#ifndef NEURON
#define NEURON

#include <stdlib.h>
#include <stdio.h>

typedef struct neuron neuron;

struct neuron
{
  int nbEntries;
  double *entries;
  double *weight;
  double output;
  double epsilon;
    double delta;
};

neuron *initNeuron(int nbEntries);
void calculateOutput(neuron *neuron);
void freeMemory(neuron *entry);

#endif
