#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "filters.h"
#define PI 3.14159265358979323846

double toRad(double a)
{
    return a * PI / 180;
}
int maxTheta (int **hough, int w, int h)
{
    int max = 0, thetaMax = 0;
    for (int r = 0; r < h; r++)
    {
        for (int t = 0; t < w; t++)
        {
            if (hough[r][t] > max)
            {
                max = hough[r][t];
                thetaMax = t;
            }
        }
    }
    return thetaMax - 90;
}

int houghHist (SDL_Surface *s)
{
    double w = s->w, h = s->h;
    int maxRho = sqrt(w*w + h*h);

    int **hough = malloc(maxRho * 2 * sizeof(int*));
    for (int i = 0; i < maxRho * 2; i++)
    {
        hough[i] = malloc(180 * sizeof(int));
        for (int j = 0; j < 180; j++)
            hough[i][j] = 0;
    }
    
    SDL_LockSurface(s);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            Uint8 color;
            SDL_GetRGB(getPixel(s, x, y), s->format,
                    &color, &color, &color);
            if (!color)
            {
                for(double theta = 0; theta <= 180; theta++)
                {
                    double radTheta = toRad(theta);
                    double rho = (double)x * cos(radTheta) + (double)y * sin(radTheta);
                    hough[maxRho + (int)rho][(int)theta]++;

                }
            }

        }
    }
    SDL_UnlockSurface(s);

    return maxTheta(hough, 180, maxRho * 2);
}
