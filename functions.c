#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "functions.h"
#include "filters.h"

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

Uint8 **matrixFromSurface(SDL_Surface *surface)
{
  Uint8** matrix = malloc(surface->w * sizeof(Uint8 *));
  for(int i = 0; i < surface->w; i++)
    matrix[i] = malloc(surface->h * sizeof(Uint8 *));
  for(int i = 0; i < surface->w; i++)
  {
    for(int j = 0; j < surface->h; j++)
    {
      Uint32 pixel = getPixel(surface, i, j);
      Uint8 r, g, b;
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      if(r == 232 && g == 0 && b == 52)
	matrix[i][j] = 10; //red
      else if(r == 5 && g  == 199 & b == 206)
	matrix[i][j] = 20; //blue
      else
	matrix[i][j] = g;
    }
  }
  return matrix;
}

char *arrayFromSurface(SDL_Surface *surface, Uint8 **pixMatrix)
{
  char* array = malloc(surface->w * surface->h * sizeof(Uint8 *));
  int n = 0;
  for(int i = 0; i < surface->w; i++)
  {
    for(int j = 0; j < surface->h; j++)
    {
      array[n] = (char)pixMatrix[i][j];
      n++;
    }
  }
  return array;
}

void getImageData(SDL_Surface *surface, Uint8 **pixMatrix)
{
  FILE* xpmFile = NULL;
  xpmFile = fopen("data.xpm", "r+");
  if(xpmFile != NULL)
  {
    fputs("#ifndef XPM\n", xpmFile);
    fputs("#define XPM\n", xpmFile);

    fputs("static char *imageData[] ={\n", xpmFile);
    fprintf(xpmFile, "\"%d %d 4 1\",\n",surface->w, surface->h);
    fputs("\"X c black\",\n", xpmFile);
    fputs("\"O c white\",\n", xpmFile);
    fputs("\"R c e80034\",\n", xpmFile);
    fputs("\"B c 05c7ce\",\n", xpmFile);
    for(int i = 0; i < surface->w; i++)
    {
      fputs("\"", xpmFile);
      for(int j = 0; j < surface->h; j++)
      {
	if(pixMatrix[i][j] == 0)
	  fputs("X", xpmFile);
	if(pixMatrix[i][j] == 255)
	  fputs("O", xpmFile);
	if(pixMatrix[i][j] == 10)
	  fputs("R", xpmFile);
	if(pixMatrix[i][j] == 20)
	  fputs("B", xpmFile);
      }
      fputs("\"\n", xpmFile);
    }
    fputs("};", xpmFile);
    fputs("#endif", xpmFile);
    fclose(xpmFile);
  }
  else
    printf("Impossible to open or read imageData.xpm");
}
