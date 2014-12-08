#ifndef FUNCTIONS
#define FUNCTIONS

#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include "network.h"

typedef struct
{
  GtkWidget *image;
  GtkWidget *text;
  GtkWidget *status;
  network *nn;
  int count;
}Zone;

SDL_Surface *copySurface(SDL_Surface *surface);
void sortArray(Uint8 array[], int size);
void printAllocError(void);
Uint8 **matrixFromSurface(SDL_Surface *surface);
char *arrayFromSurface(SDL_Surface *surface, Uint8 **pixMatrix);
void getImageData(SDL_Surface *surface, Uint8 **pixMatrix);
void cb_learn(GtkWidget *widget, gpointer data);
void cb_process(GtkWidget *widget, gpointer data);

#endif
