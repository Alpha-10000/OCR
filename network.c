#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "network.h"

const int NN_RESOLUTION = 16;
const int NN_NBOUTPUTS = 93;

network *initNetwork(int nbLayers, int nbNeurons)
{
  int nbOutPut = NN_NBOUTPUTS;
  int nbInputs = NN_RESOLUTION*NN_RESOLUTION;
  network *result = malloc(sizeof(network));
  result->nbLayers = nbLayers;
  result->layers = malloc((result->nbLayers)*sizeof(layer*));

  result->layers[0] = initLayer(nbNeurons, nbInputs);

  for(int i = 1; i < nbLayers-1; i++)
    result->layers[i] = initLayer(nbNeurons, result->layers[i-1]->nbNeurons);

  result->layers[nbLayers-1] =
    initLayer(nbOutPut, result->layers[nbLayers-2]->nbNeurons);

  result->output = malloc(nbOutPut*sizeof(double));

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
  int toBeConverted = 0;
  double maxvalue = 0;
  for (int i = 0; i < network->layers[network->nbLayers-1]->nbNeurons; i++)
  {
    if (network->output[i] > maxvalue)
    {
      maxvalue = network->output[i];
      toBeConverted = i;
    }
    printf("final output (%d)=%f\n", i, network->output[i]);
  }
  printf("\nmax value = %f\ntoBeConverted = %d\n", maxvalue, toBeConverted);
}

char getNNcharOutput(network *network)
{
  int toBeConverted = 0;
  double maxvalue = 0;
  for (int i = 0; i < network->layers[network->nbLayers-1]->nbNeurons; i++)
  {
    if (network->output[i] > maxvalue)
    {
      maxvalue = network->output[i];
      toBeConverted = i;
    }
  }

  if (toBeConverted == 0)
    toBeConverted = 33;
  else
    toBeConverted += 34;
  return (char)toBeConverted;
}

char *readText(network *network, SDL_Surface *surface,
	       Block *blocks, int nbLines, char *text)
{
  int *entryVector = malloc(NN_RESOLUTION*NN_RESOLUTION*sizeof(int));
  int currentChar = 0;
  for (int i = 0; i < nbLines; i++)
  {
    for (int j = 0; j < blocks[i].nbChars; j++)
    {
      //printf("Reading char(%d) line(%d) pos(%d) : ", currentChar,
      //  getLineNb(currentChar, blocks, nbLines),
      //  getCharNb(currentChar, blocks, nbLines));
      fillEntryVector(surface, entryVector,
		      getCharNb(currentChar, blocks, nbLines),
		      getLineNb(currentChar, blocks, nbLines));
      computeOutput(network, entryVector);
      //printf("Exiting readtext   ");
      char c = getNNcharOutput(network);
      char s[2];
      s[0] = c;
      s[1] = '\0';
      strcat(text, s);
      printf("%s", s);
      currentChar++;
    }
    strcat(text, "\n");
    printf("\n");
  }
  printf("\n");
  strcat(text, "\n");
  free(entryVector);
  return text;
}

void fillEntryVector(SDL_Surface *surface, int *entryVector, int charNb, int LineNb)
{
  Uint32 pixel;
  Uint8 grey;
  for (int x = 0; x < NN_RESOLUTION; x++)
  {
    for (int y = 0; y < NN_RESOLUTION; y++)
    {
      pixel = getPixel(surface, charNb*NN_RESOLUTION+x, LineNb*NN_RESOLUTION+y);
      SDL_GetRGB(pixel, surface->format, &grey, &grey, &grey);
      entryVector[x*NN_RESOLUTION+y] = 1;
      if (grey == 0)
        entryVector[x*NN_RESOLUTION+y] = 0;
    }
  }
}

int getLineNb(int entry, Block* blocks, int nbLines)
{
  int result = 0;
  int counter = 0;
  for (result = 0; result < nbLines && counter <= entry; result++)
    counter+= blocks[result].nbChars;

  //printf("Line(%d)...", result-1);
  return result-1;
}

int getCharNb(int entry, Block* blocks, int nbLines)
{
  int result = 0;
  for (result = 0; result <= nbLines; result++)
  {
    if (entry < blocks[result].nbChars)
    {
      //printf("Char(%d) ", entry);
      return entry;
    }
    entry -= blocks[result].nbChars;
  }
  printf("\nWARNING: RETURN VALUE OF -1 FOR getCharNb IN NETWORK.C\n");
  return -1;
}

void learnNetwork(network *network, Block *blocks, SDL_Surface *surface, int nbLines)
{
  //Assuming the text is abcd....xyzABCD...XYZ
  int *entryVector = malloc(NN_RESOLUTION*NN_RESOLUTION*sizeof(int));
  double learnCoef = 1.5;

  for (int nbTest = 0; nbTest < 2000; nbTest++)
  {
    for (int currentTest = 0; currentTest < NN_NBOUTPUTS; currentTest++)
    {
      fillEntryVector(surface, entryVector,
        getCharNb(currentTest, blocks, nbLines),
        getLineNb(currentTest, blocks, nbLines));

      computeOutput(network, entryVector);

      for (int i = 0; i < network->layers[network->nbLayers-1]->nbNeurons; i++)
      {
       network->layers[network->nbLayers-1]->neurons[i]->delta =
       network->output[i]*(1-network->output[i])*
       ((currentTest == i ? 1 : 0)-network->output[i]);
     }
     for (int i = network->nbLayers-2; i >= 0; i--)
     {
       for(int j = 0; j < network->layers[i]->nbNeurons; j++)
       {
         double backPropCoef = 0;
         for(int k = 0; k < network->layers[i+1]->nbNeurons; k++)
         {
           backPropCoef += network->layers[i+1]->neurons[k]->delta*
           network->layers[i+1]->neurons[k]->weight[j];
         }
         network->layers[i]->neurons[j]->delta =
         network->layers[i]->neurons[j]->output
         * (1-network->layers[i]->neurons[j]->output)
         * backPropCoef;
       }
     }
     for (int i = 0; i < network->nbLayers; i++)
     {
       for (int j = 0; j < network->layers[i]->nbNeurons; j++)
       {
         for (int k = 0; k < network->layers[i]->neurons[j]->nbEntries; k++)
         {
           network->layers[i]->neurons[j]->weight[k] += learnCoef
           * network->layers[i]->neurons[j]->delta
           * network->layers[i]->neurons[j]->entries[k];
         }
       }
     }
   }
 }

 free(entryVector);
}

void computeOutput(network *network, int *entryVector)
{
  //printf("Computing output for the first layer\n");
  for (int i = 0; i < network->layers[0]->nbNeurons; i++)
  {
    for (int x = 0; x < NN_RESOLUTION; x++)
      for (int y = 0; y < NN_RESOLUTION; y++)
        network->layers[0]->neurons[i]->entries[x*NN_RESOLUTION + y] = entryVector[x*NN_RESOLUTION + y];

    calculateOutput(network->layers[0]->neurons[i]);
  }
  for (int i = 1; i < network->nbLayers; i++) // For each layer
  {
    //printf("Entering the layer %d\n", i);
    // for each neuron in the layer
    for (int j = 0; j < network->layers[i]->nbNeurons; j++)
    {
      //printf("Entering the neuron %d\n", j);
      // for each output in the previous layer
      for(int k = 0; k < network->layers[i-1]->nbNeurons; k++)
      {
	       //printf("Accessing the %d neuron on previous layer\n",k);
	       network->layers[i]->neurons[j]->entries[k]
	         = network->layers[i-1]->neurons[k]->output;
      }
      calculateOutput(network->layers[i]->neurons[j]);
    }
  }
  for (int i = 0; i < network->layers[network->nbLayers-1]->nbNeurons; i++)
    network->output[i] = network->layers[network->nbLayers-1]->neurons[i]->output;
}

void freeNetwork(network *network)
{
  for (int i = 0; i < network->nbLayers; i++)
    freeLayer(network->layers[i]);
  free(network->layers);
  free(network->output);
  free(network);
}
