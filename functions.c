#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include <wchar.h>
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
  for(int i = 0; i < size-1; i++)
  {
    int min = i;
    for(int j = i + 1; j < size; j++)
    {
      if(array[j] < array[min])
	min = j;
    }
    if(min != i)
    {
      Uint8 temp = array[min];
      array[min] = array[i];
      array[i] = temp;
    }
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
  if(pixBuf && zone->count == 0)
  {
    gdk_pixbuf_save(pixBuf, "data.bmp", "bmp", NULL, NULL, NULL);
    SDL_Surface *textImage = NULL;
    textImage = IMG_Load("data.bmp");
    resizeImage(zone->image);
    if(textImage == NULL)
    {
      fprintf(stderr, "Error while loading SDL Surface\n");
      exit(EXIT_FAILURE);
    }
    double angle = houghHist(textImage);
    if(angle != 0)
    {
      SDL_Surface *swp = textImage;
      textImage = rotate(textImage, -angle);
      SDL_FreeSurface(swp);
    }

    greyScale(textImage);
    // noiseRemove(textImage);
    binarize(textImage);

    int nbLines;
    Block *blocks = findBlocks(textImage, &nbLines);
    findChars(textImage, blocks, nbLines);

    textImage = resizeChars(textImage, blocks, nbLines);
    readNetworkSettings(zone->nn);

    wchar_t* chars = NULL;
    chars = malloc(nbLines * 200 * sizeof(wchar_t));
    for(int i = 0; i < 200 * nbLines; i++)
      chars[i] = L'\0';
    chars = readText(zone->nn, textImage, blocks, nbLines, chars);
    displayOutput(chars, zone);
    zone->count++;
    freeBlocks(blocks, nbLines);
    SDL_FreeSurface(textImage);
    free(chars);
  }
  (void)widget;
}
