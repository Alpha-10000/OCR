#include <stdio.h>
#include <stdlib.h>
#include "save.h"

void saveNetworkSettings(network *network)
{
  if (network)
  {
    FILE *file;
    if ( (file = fopen("NNsettings", "w")) == NULL)
      err(3, "Error while creating NNsettings\n");

    for (int i = 0; i < network->nbLayers; i++)
    {
      for (int j = 0; j < network->layers[i]->nbNeurons; j++)
      {
	for (int k = 0; k < network->layers[i]->neurons[j]->nbEntries; k++)
	{
	  fprintf(file, "%.8f\n", network->layers[i]->neurons[j]->weight[k]);
	}
      }
    }
    fclose(file);
  }
  else
    printf("Please give a non-NULL argument to saveNetworkSettings\n");
}

void readNetworkSettings(network *network)
{
  if (network)
  {
    FILE *file;
    float weight;

    if ( (file = fopen("NNsettings", "r")) == NULL)
      err(3, "Error while trying to read NNsettings\n");

    for (int i = 0; i < network->nbLayers; i++)
    {
      for (int j = 0; j < network->layers[i]->nbNeurons; j++)
      {
	for (int k = 0; k < network->layers[i]->neurons[j]->nbEntries; k++)
	{
	  fscanf(file,"%f",&weight);
	  network->layers[i]->neurons[j]->weight[k] = weight;
	}
      }
    }

    fclose(file);
  }
  else
    printf("Please give a non-NULL argument to readNetworkSettings\n");
}
