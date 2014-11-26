#ifndef FUNCTIONS
#define FUNCTIONS
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include "functions.h"
#include "filters.h"
#include "network.h"
#include "detection.h"
#include "gui.h"

SDL_Surface *copySurface(SDL_Surface *surface)
{
  SDL_Surface *copy = NULL;
  copy = SDL_CreateRGBSurface(
    SDL_HWSURFACE, surface->w, surface->h, 32,
    surface->format->Rmask, surface->format->Gmask,
    surface->format->Bmask, surface->format->Amask);
  if(surface == NULL || copy == NULL)
    return NULL;
  SDL_FreeSurface(copy);
  copy = NULL;
  return SDL_DisplayFormatAlpha(surface);
}
void sortArray(Uint8 array[], int size)
{
  for(int i = 0; i < size; i++)
  {
    Uint8 temp = array[i];
    int j = i;
    while(j >= 0 && array[j-1] > temp)
    {
      array[j] = array[j-1];
      j--;
    }
    array[j] = temp;
  }
}

void printAllocError(void)
{
  printf("Error : alloc fail");
  exit(EXIT_FAILURE);
}

void cb_process(GtkWidget *widget, gpointer data)
{
  GtkWidget *image = NULL;
  image = GTK_WIDGET(data);
  GdkPixbuf *pixBuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
  gdk_pixbuf_save(pixBuf, "data.bmp", "bmp", NULL, NULL, NULL);

  SDL_Surface *text = NULL;
  text = IMG_Load("data.bmp");
  if(text == NULL)
  {
    fprintf(stderr, "Give to the program an image as argument\n");
    exit(EXIT_FAILURE);
  }

  greyScale(text);
  ///noiseRemove(text);
  binarize(text);

  int nbLines;
  Block *blocks = findBlocks(text, &nbLines);
  //print_blocks(blocks, nbLines);
  findChars(text, blocks, nbLines);
  //drawLinesChars(text, blocks, nbLines);
  ///SDL_Surface *resized = NULL;
  text = resizeChars(text, blocks, nbLines);
  //Neural Network tests
  network *testNN = initNetwork(3,30);
  learnNetwork(testNN, blocks, text, nbLines);
  /*
    int *entryVector = malloc(NN_RESOLUTION*NN_RESOLUTION*sizeof(int));
    fillEntryVector(text, entryVector,
    getCharNb(0, blocks, nbLines),
    getLineNb(0, blocks, nbLines));
    computeOutput(testNN, entryVector);
    printOutput(testNN);
    free(entryVector);
  */
  readText(testNN, text, blocks, nbLines);
  freeNetwork(testNN);
  freeBlocks(blocks, nbLines);

  pixBuf = loadPixBuf(text);
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixBuf);
  SDL_FreeSurface(text);
  (void)widget;
}
#endif
