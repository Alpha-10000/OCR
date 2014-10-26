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
  double output;
};

void computeOutput(int *entry, int size, network *network);
network *initNetwork(int nbLayers, int nbNeurons, int entryPerNeurons);
void printEverything(network *network);
void printOutput(network *network);
void learnNetwork(int entry[4][2], int expected[4], network *network);
void computeOutput(int *entry, int size, network *network);
void freeNetwork(network *network);
#endif
