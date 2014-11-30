#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "filters.h"
#define max(a,b) (a>=b?a:b)
#define PI 3.14159265358979323846

int houghHist(SDL_Surface *surface)
{
	int *teta_list = malloc(180 * sizeof(int));
	int goodteta = 0;
	SDL_LockSurface(surface);
	for (int j = 0; j < surface->h; j++)
	{
		Uint8 color;
		SDL_GetRGB(getPixel(surface, (surface->w)/2, j), surface->format,
		 &color, &color, &color);
		if (!color)
		{
			//pour tous les teta de 0 à 180 degré, tracer une ligne et compter le nb de
			//pixels noirs sur chaque ligne et stocker ce nb dans une matrice(le pix,teta)
			int teta, rho, nbpixels = 0;
			for (int indexteta = 0; indexteta<180; indexteta++)
			{
				teta = (indexteta/180)*PI;
				rho = (surface->w) / 2 * cos(teta) + j * sin(teta);
				//calculer nb pixels sur la ligne courante
				if (nbpixels) //< nb pixels compté
					goodteta = teta;
			} 
		}
		teta_list[goodteta]++;
	}
	int return_teta = 0;
	for (int i = 0; i < 180; i++)
	{
		return_teta = max(return_teta, teta_list[i]);
	}
	return return_teta;
}
