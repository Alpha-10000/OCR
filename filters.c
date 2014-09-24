#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "filters.h"

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
	    if (i == 0 && j == 0)
	    {
		int re = (int)r;
		int ge = (int)g;
		int be = (int)b;
		printf("r: %d g: %d b: %d", re, ge, be);
	    }
	}
    }
    SDL_UnlockSurface(surface);
}
