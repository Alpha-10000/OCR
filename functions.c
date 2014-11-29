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
  Zone *zone = (Zone*)data;
  GtkWidget * image = NULL;
  image = GTK_WIDGET(zone->image);
  GdkPixbuf *pixBuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
  gdk_pixbuf_save(pixBuf, "data.bmp", "bmp", NULL, NULL, NULL);

  SDL_Surface *textImage = NULL;
  textImage = IMG_Load("data.bmp");
  if(textImage == NULL)
  {
    fprintf(stderr, "Error while loading SDL Surface\n");
    exit(EXIT_FAILURE);
  }
  
  greyScale(textImage);
  //noiseRemove(text);
  binarize(textImage);
  rotate(textImage,30);
 
  int nbLines;
  Block *blocks = findBlocks(textImage, &nbLines);
  //print_blocks(blocks, nbLines);
  findChars(textImage, blocks, nbLines);
  //drawLinesChars(text, blocks, nbLines);
  ///SDL_Surface *resized = NULL;
  textImage = resizeChars(textImage, blocks, nbLines);
  //Neural Network tests
  network *testNN = initNetwork(3,30);
  learnNetwork(testNN, blocks, textImage, nbLines);
  /*
    int *entryVector = malloc(NN_RESOLUTION*NN_RESOLUTION*sizeof(int));
    fillEntryVector(text, entryVector,
    getCharNb(0, blocks, nbLines),
    getLineNb(0, blocks, nbLines));
    computeOutput(testNN, entryVector);
    printOutput(testNN);
    free(entryVector);
  */
  char* chars = malloc(nbLines * 1000 * sizeof(char));

  readText(testNN, textImage, blocks, nbLines, chars);
  freeNetwork(testNN);
  freeBlocks(blocks, nbLines);

  pixBuf = loadPixBuf(textImage);
  gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixBuf);

  GtkTextBuffer *textBuffer = NULL;
  textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(zone->text));
  GtkTextIter iter;
  gtk_text_buffer_get_iter_at_line(textBuffer, &iter, 0);
  gchar *text = NULL;
  text = g_locale_to_utf8(chars, -1, NULL, NULL, NULL);
  gtk_text_buffer_insert(textBuffer, &iter, text, -1);

  SDL_FreeSurface(textImage);
  free(chars);
  (void)widget;
}
