#include <stdlib.h>
#include <stdio.h>
#include "network.h"

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
        printf("\n\nLayer %d //////%d/////\n", i,network->layers[i]->nbNeurons);
        for(int j = 0; j < network->layers[i]->nbNeurons; j++)
        {
            printf("\033[01;37m Neuron %d: nbEntries=%d \033[01;32m output=%f\n",j,
                    network->layers[i]->neurons[j]->nbEntries,
                    network->layers[i]->neurons[j]->output);
            for(int k = 0; k < network->layers[i]->neurons[j]->nbEntries; k++)
            {
                printf("\033[22;31m weight %d=%f ",k,
                        network->layers[i]->neurons[j]->weight[k]);
            }
            printf("\033[01;37m \n");
        }
    }
}

void printOutput(network *network)
{
    printf("final output=%f\n", network->layers[network->nbLayers-1]->
            neurons[0]->output);
}

void learnNetwork(int entry[4][2], int expected[4], network *network)
{
    for (int nbTest = 0; nbTest < 10; nbTest++)
    {
        for (int sizeA = 0; sizeA < 4; sizeA++)
        {
            computeOutput(entry[sizeA], 2, network);
            double savedOutput = network->output;
            for (int i = 0; i < network->layers[network->nbLayers-1]->nbNeurons; i++)
            {
                network->layers[network->nbLayers-1]->neurons[i]->delta =
                    savedOutput*(1-savedOutput)*(expected[sizeA]-savedOutput); 
            }
            for (int i = network->nbLayers-2; i >= 0; i--)
            {
                for(int j = 0; j < network->layers[i]->nbNeurons; j++)
                {
                    double backPropCoef = 0;
                    for(int k = 0; k < network->layers[i+1]->nbNeurons; k++)
                        backPropCoef += network->layers[i+1]->neurons[k]->delta*
                            network->layers[i+1]->neurons[k]->weight[j];

                    network->layers[i]->neurons[j]->delta =
                        savedOutput*(1-savedOutput)*backPropCoef;
                }
            }
            for (int i = 0; i < network->nbLayers; i++)
            {
                for (int j = 0; j < network->layers[i]->nbNeurons; j++)
                {
                    for (int k = 0; k < network->layers[i]->neurons[j]->nbEntries; k++)
                    {
                        network->layers[i]->neurons[j]->weight[k] +=
                            1*network->layers[i]->neurons[j]->delta*
                            network->layers[i]->neurons[j]->entries[k];
                    }
                }
            }
        }
    }
}

void computeOutput(int *entry, int size, network *network)
{
    //printf("Computing output for the first layer\n");
    for (int i = 0; i < network->layers[0]->nbNeurons; i++)
    {
        for(int j = 0; j < size; j++)
        {
            network->layers[0]->neurons[i]->entries[j] = entry[j];
        }
        calculateOutput(network->layers[0]->neurons[i]);
    }
    for (int i = 1; i < network->nbLayers; i++) // For each layer
    {
        //printf("Entering the layer %d\n", i);
        for (int j = 0; j < network->layers[i]->nbNeurons; j++) // for each neuron in the layer
        {
            //printf("Entering the neuron %d\n", j);
            for(int k = 0; k < network->layers[i-1]->nbNeurons; k++) // for each output 
                // in the previous layer
            {
                //printf("Accessing the %d neuron on previous layer\n",k);
                network->layers[i]->neurons[j]->entries[k] 
                    = network->layers[i-1]->neurons[k]->output;
            }
            calculateOutput(network->layers[i]->neurons[j]);
        }
    }
    network->output = network->layers[network->nbLayers-1]->neurons[0]->output;
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
