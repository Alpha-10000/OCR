#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "filters.h"
#define PI 3.14159265358979323846

int houghHist(SDL_Surface *surface)
{
    //initialisation d'une matrice [maxrho][180]
    int maxrho = sqrt(surface->w * surface->w + surface->h * surface->h);
    int **hough_mat = malloc(maxrho * sizeof(int*));

    for (int i = 0; i < maxrho; i++)
        hough_mat[i] = malloc(180 * sizeof(int));

    //parcours de l'image
    SDL_LockSurface(surface);
    
    for (int x = 0; x < surface->w; x++)
    {
        for (int y = 0; y < surface->h; y++)
        {
            Uint8 color;
            SDL_GetRGB(getPixel(surface, x, y), surface->format,
                    &color, &color, &color);
            if (!color)
            {
                int rho;
                double teta;
                //parcours de tous les angles du pixel noir
                for (int indexteta = 0; indexteta < 180; indexteta++)
                {
                    //transformation en coordonnées polaires
                    teta = (indexteta / 180)*PI;
                    rho = x * cos(teta) + y * sin(teta);
                    //incrémentation de la case correspondante dans la matrice
                    hough_mat[rho][indexteta]++;
                }
            }
        }
    }
    SDL_UnlockSurface(surface);
    	int max = 0, winteta = 0;
	for (int r = 0; r < maxrho; r++)
	{
		for (int t = 0; t < 180; t++)
		{
			if (hough_mat[r][t] > max)
			{
				max = hough_mat[r][t];
				winteta = t;
			}
		}
	}	
    return winteta;
}

//XX Il faut free !!!!!
