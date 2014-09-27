#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "filters.h"
#include "functions.h"

Uint32 getPixel(SDL_Surface *surface, int x, int y) //From SDL doc
{
    int bpp = surface->format->BytesPerPixel;
    /* p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
	return *p;

    case 2:
	return *(Uint16 *)p;

    case 3:
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
	    return p[0] << 16 | p[1] << 8 | p[2];
	else
	    return p[0] | p[1] << 8 | p[2] << 16;

    case 4:
	return *(Uint32 *)p;

    default:
	return 0;       /* shouldn't happen, but avoids warnings */
    }
}

void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    /* p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
	*p = pixel;
	break;

    case 2:
	*(Uint16 *)p = pixel;
	break;

    case 3:
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
	    p[0] = (pixel >> 16) & 0xff;
	    p[1] = (pixel >> 8) & 0xff;
	    p[2] = pixel & 0xff;
	} else {
	    p[0] = pixel & 0xff;
	    p[1] = (pixel >> 8) & 0xff;
	    p[2] = (pixel >> 16) & 0xff;
	}
	break;

    case 4:
	*(Uint32 *)p = pixel;
	break;
    }
}


void greyScale(SDL_Surface *surface)
{
    Uint8 r, g, b, grey;
    SDL_LockSurface(surface); //Necessary before using getPixel
    int i;
    for(i = 0; i < surface->w; i++)
    {
	int j;
	for(j = 0; j < surface->h; j++)
	{  
	    Uint32 pixel = getPixel(surface, i, j);
	    SDL_GetRGB(pixel, surface->format, &r, &g, &b);
	    grey = r * 0.3 + g * 0.59 + b * 0.11;
	    pixel = SDL_MapRGB(surface->format, grey, grey, grey);
	    setPixel(surface, i, j, pixel);
	}
    }
    SDL_UnlockSurface(surface);
}

void noiseRemove(SDL_Surface *surface)
{
    SDL_Surface *copy;
    //copy = copySurface(surface);
    copy = SDL_ConvertSurface(surface, surface->format, SDL_HWSURFACE);
    if(surface != NULL && copy != NULL)
    {
	Uint8 grey;
	SDL_LockSurface(surface);
	SDL_LockSurface(copy);
	
	int i;
	for(i = 0; i < surface->w; i++)
	{
	    int j;
	    for(j = 0; j < surface->h; j++)
	    {
		Uint32 pixelMatrix[9];
		Uint32 pixel = getPixel(copy, i, j);
		SDL_GetRGB(pixel, copy->format, &grey, &grey, &grey);
		if(i > 0 && i < (surface->w - 1) && j > 0 && j < (surface->h - 1) )
		{
		    SDL_GetRGB(getPixel(copy, i-1, j-1), copy->format, &grey, &grey, &grey);
		    pixelMatrix[0] = grey;
		    SDL_GetRGB(getPixel(copy, i-1, j), copy->format, &grey, &grey, &grey);
		    pixelMatrix[1] = grey;
		    SDL_GetRGB(getPixel(copy, i-1, j+1), copy->format, &grey, &grey, &grey);
		    pixelMatrix[2] = grey;
		    SDL_GetRGB(getPixel(copy, i, j-1), copy->format, &grey, &grey, &grey);
		    pixelMatrix[3] = grey;
		    SDL_GetRGB(getPixel(copy, i, j), copy->format, &grey, &grey, &grey);
		    pixelMatrix[4] = grey;
		    SDL_GetRGB(getPixel(copy, i, j+1), copy->format, &grey, &grey, &grey);
		    pixelMatrix[5] = grey;
		    SDL_GetRGB(getPixel(copy, i+1, j-1), copy->format, &grey, &grey, &grey);
		    pixelMatrix[6] = grey;
		    SDL_GetRGB(getPixel(copy, i+1, j), copy->format, &grey, &grey, &grey);
		    pixelMatrix[7] = grey;
		    SDL_GetRGB(getPixel(copy, i+1, j+1), copy->format, &grey, &grey, &grey);
		    pixelMatrix[8] = grey;
		}
		
		sortArray(pixelMatrix, 9);
		grey = pixelMatrix[4];
		pixel = SDL_MapRGB(copy->format, grey, grey, grey);
		setPixel(surface, i, j, pixel);
	    }
	}
	SDL_UnlockSurface(copy);
	SDL_UnlockSurface(surface);
    }
}

void binarize(SDL_Surface *surface) //threshold must be computed for better result
{
    Uint8 grey;
    SDL_LockSurface(surface);
    int i;
    for(i = 0; i < surface->w; i++)
    {
	int j;
	for(j = 0; j < surface->h; j++)
	{  
	    Uint32 pixel = getPixel(surface, i, j);
	    SDL_GetRGB(pixel, surface->format, &grey, &grey, &grey);
	    if (grey > 128)
		grey = 255;
	    else
		grey = 0;
	    pixel = SDL_MapRGB(surface->format, grey, grey, grey);
	    setPixel(surface, i, j, pixel);
	}
    }
    SDL_UnlockSurface(surface);
}
