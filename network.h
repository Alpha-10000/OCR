#ifndef NETWORK
#define NETWORK

#include <stdio.h>
#include <stdlib.h>
#include "layer.h"

typedef struct network network;

struct network
{
    int nbLayers;
    layer* *layers;
};

network *initNetwork(int nbLayers, int nbNeurons, int entryPerNeurons)
{
    network *result = malloc(sizeof(network));
    result->nbLayers = nbLayers;
    result->layers = malloc((result->nbLayers)*sizeof(layer*));
    result->layers[0] = initLayer(nbNeurons, entryPerNeurons);
    for(int i = 1; i < nbLayers-1; i++)
        result->layers[i] = initLayer(nbNeurons, result->layers[i-1]->nbNeurons);
    result->layers[nbLayers-1] = initLayer(1,result->layers[nbLayers-2]->nbNeurons);

    return result;
}

void printEverything(network *network)
{
    for(int i = 0; i < network->nbLayers; i++)
    {
        printf("Layer %d //////%d/////\n", i,network->layers[i]->nbNeurons);
        for(int j = 0; j < network->layers[i]->nbNeurons; j++)
        {
            printf("Neuron %d: nbEntries=%d output=%f\n",j,
                    network->layers[i]->neurons[j]->nbEntries,
                    network->layers[i]->neurons[j]->output);
        }
    }
}

void computeOutput(int *entry, int size, network *network)
{
    for (int i = 0; i < network->layers[0]->nbNeurons; i++)
    {
        printf("Doing the first layer\n");
        for(int j = 0; j < size; j++)
        {
            network->layers[0]->neurons[i]->entries[j] = entry[j];
        }
        calculateOutput(network->layers[0]->neurons[i]);
    }
    for (int i = 1; i < network->nbLayers; i++) // For each layer
    {
        printf("Entering the layer %d\n", i);
        for (int j = 0; j < network->layers[i]->nbNeurons; j++) // for each neuron in the layer
        {
            printf("Entering the neuron %d\n", j);
            for(int k = 0; k < network->layers[i-1]->nbNeurons; k++) // for each output 
                // in the previous layer
            {
                printf("Accessing the %d neuron on previous layer\n",k);
                network->layers[i]->neurons[j]->entries[k] 
                    = network->layers[i-1]->neurons[k]->output;
            }
            printf("DEBUGLINE\n");
            calculateOutput(network->layers[i]->neurons[j]);
        }
    }
}
/*
   void learnNetwork(network *network)
   {

   }
   */
void freeNetwork(network *network)
{
    for (int i = 0; i < network->nbLayers; i++)
        freeLayer(network->layers[i]);
    free(network->layers);
    free(network);
}

#endif
