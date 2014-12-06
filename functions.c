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
#include "rotation.h"
#include "save.h"
#include "angle.h"

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

void cb_learn(GtkWidget *widget, gpointer data)
{
  Zone *zone = (Zone*)data;
  GdkPixbuf *pixBuf = NULL;
  pixBuf = gtk_image_get_pixbuf(GTK_IMAGE(zone->image));
  if(pixBuf)
  {
    gdk_pixbuf_save(pixBuf, "data.bmp", "bmp", NULL, NULL, NULL);
    SDL_Surface *textImage = NULL;
    textImage = IMG_Load("data.bmp");
    if(textImage == NULL)
    {
      fprintf(stderr, "Error while loading SDL Surface\n");
      exit(EXIT_FAILURE);
    }
    freeNetwork(zone->nn);
    zone->nn = initNetwork(3, 40);
    greyScale(textImage);
    binarize(textImage);
    int nbLines = 0;
    Block *blocks = findBlocks(textImage, &nbLines);
    findChars(textImage, blocks, nbLines);
    textImage = resizeChars(textImage, blocks, nbLines);
    learnNetwork(zone->nn, blocks, textImage, nbLines);
    saveNetworkSettings(zone->nn);
    freeBlocks(blocks, nbLines);
    SDL_FreeSurface(textImage);
  }
  (void)widget;
}

void cb_process(GtkWidget *widget, gpointer data)
{
  Zone *zone = (Zone*)data;
  GdkPixbuf *pixBuf = NULL;
  pixBuf = gtk_image_get_pixbuf(GTK_IMAGE(zone->image));
  if(pixBuf)
  {
    gdk_pixbuf_save(pixBuf, "data.bmp", "bmp", NULL, NULL, NULL);
    SDL_Surface *textImage = NULL;
    textImage = IMG_Load("data.bmp");
    if(textImage == NULL)
    {
      fprintf(stderr, "Error while loading SDL Surface\n");
      exit(EXIT_FAILURE);
    }

    /*double angle = houghHist(textImage);
      printf("Angle = %f\n", angle);
      SDL_Surface *swp = textImage;
      textImage = rotate(textImage, -angle);
      SDL_FreeSurface(swp);*/

    greyScale(textImage);
    //noiseRemove(textImage);
    binarize(textImage);

    pixBuf = loadPixBuf(textImage);
    gtk_image_set_from_pixbuf(GTK_IMAGE(zone->image), pixBuf);

    int nbLines;
    Block *blocks = findBlocks(textImage, &nbLines);
    //print_blocks(blocks, nbLines);
    findChars(textImage, blocks, nbLines);

    //printMedianCharSize(blocks, nbLines);
    //printSpaces(blocks, nbLines);
    //drawLinesChars(textImage, blocks, nbLines);

    textImage = resizeChars(textImage, blocks, nbLines);
    readNetworkSettings(zone->nn);
    //learnNetwork(zone->nn, blocks, textImage, nbLines);

    //pixBuf = loadPixBuf(textImage);
    //gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixBuf);
    /*
      int *entryVector = malloc(NN_RESOLUTION*NN_RESOLUTION*sizeof(int));
      fillEntryVector(textImage, entryVector,
      getCharNb(15, blocks, nbLines),
      getLineNb(15, blocks, nbLines));
      computeOutput(testNN, entryVector);
      printOutput(testNN);
      free(entryVector);
    */
    //saveNetworkSettings(zone->nn);

    char* chars = NULL;
    chars = malloc(nbLines * 200 * sizeof(char));
    for(int i = 0; i < 200 * nbLines; i++)
      chars[i] = '\0';
    chars = readText(zone->nn, textImage, blocks, nbLines, chars);
    displayOutput(chars, zone);

    freeBlocks(blocks, nbLines);
    SDL_FreeSurface(textImage);
    free(chars);
  }
  (void)widget;
}
