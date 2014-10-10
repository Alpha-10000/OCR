#ifndef LAYER
#define LAYER

#include <stdio.h>
#include <stdlib.h>
#include "neuron.h"

typedef struct layer layer;

struct layer
{
    int nbNeurons;
    neuron* *neurons;
    double *entries;
    double *outputs;
};

layer* initLayer(int nbNeurons, int entriesPerNeuron);
void freeLayer(layer *layer);

#endif
