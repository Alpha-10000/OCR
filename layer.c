#include <stdio.h>
#include <stdlib.h>
#include "layer.h"
#include "neuron.h"

layer *initLayer(int nbNeurons, int entriesPerNeuron)
{
  layer *result = malloc(sizeof(layer));
  result->nbNeurons = nbNeurons;
  result->entries = malloc(entriesPerNeuron*sizeof(double));
  result->outputs = malloc(nbNeurons*sizeof(double));
  result->neurons = malloc(nbNeurons*sizeof(neuron*));
  for (int i = 0; i < nbNeurons; i++)
    result->neurons[i] = initNeuron(entriesPerNeuron);
  return result;
}

void freeLayer(layer *layer)
{
  for(int i = 0; i < layer->nbNeurons; i++)
    freeMemory(layer->neurons[i]);
  free(layer->neurons);
  free(layer->entries);
  free(layer->outputs);
  free(layer);
}
