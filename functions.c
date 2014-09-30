#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "functions.h"

void sortArray(Uint32 array[], int size)
{
    int i;
    for(i = 0; i < size; i++)
    {
	Uint32 temp = array[i];
	int j = i;
	while(j >= 0 && array[j-1] > temp)
	{
	    array[j] = array[j-1];
	    j -= 1;
	}
	array[j] = temp;
    }
}

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
