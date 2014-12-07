#ifndef NETWORK
#define NETWORK
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "layer.h"
#include "detection.h"

typedef struct network network;
struct network
{
  int nbLayers;
  layer* *layers;
  double *output;
};

extern const int NN_RESOLUTION;
extern const int NN_NBOUTPUTS;

network *initNetwork(int nbLayers, int nbNeurons);
void printEverything(network *network);
void printOutput(network *network);
char numToChar(int entry);
wchar_t getNNcharOutput(network *network);
wchar_t *readText(network *network, SDL_Surface *surface,
	       Block *blocks, int nbLines, wchar_t *text);
void fillEntryVector(SDL_Surface *surface, int *entryVector, int charNb, int LineNb);
int getLineNb(int entry, Block* blocks, int nbLines);
int getCharNb(int entry, Block* blocks, int nbLines);
void learnNetwork(network *network, Block *blocks, SDL_Surface *surface, int nbLines);
void computeOutput(network *network, int *entryVector);
void freeNetwork(network *network);
#endif
