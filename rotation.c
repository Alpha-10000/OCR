/* Rotation.c */

#include "filters.h"
#include <math.h>

#define PI 3.14159265

void rotate(SDL_Surface *old, double angle)
{
    int h0 = old->h, w0 = old->w;
    double cosA = cos(angle * PI / 180);
    double sinA = sin(angle * PI / 180);
    int w = (w0 * cosA + h0 * sinA);
    int h = (h0 * cosA+ w0 * sinA);
    int xc = w0/2, yc = h0/2;

    SDL_Surface *new = SDL_CreateRGBSurface(0, w, h, old->format->BitsPerPixel,
                                            0,0,0,0);
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int x0 = (cosA * (x - xc) + sinA * (y - yc)) + xc;
            int y0 = (-sinA * (x - xc) + cosA * (y - yc)) + yc;
            
            if (x0 >= 0 && x0 < w0 && y0 >= 0 && y0 < h0)
            {
                Uint32 pix = getPixel(old, x0, y0);
                setPixel(new, x, y, pix);
            }
            else
                setPixel(new, x, y, SDL_MapRGB(old->format, 255, 255, 255));
        }
    }
}
