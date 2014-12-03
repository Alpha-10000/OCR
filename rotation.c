/* Rotation.c */

#include "filters.h"
#include <math.h>

#define PI 3.14159265

SDL_Surface* rotate(SDL_Surface *old, double angle)
{
    int h0 = old->h, w0 = old->w;
    double cosA = cos(angle * PI / 180);
    double sinA = sin(angle * PI / 180);
    //int w = w0 * abs(cosA) + h0 * abs(sinA);
    //int h = h0 * abs(cosA) + w0 * abs(sinA);
    double x0c = w0/2, y0c = h0/2;
    //double xc = w/2, yc = h/2;

    SDL_Surface *new = SDL_CreateRGBSurface(0, w0, h0, old->format->BitsPerPixel,
                                            0,0,0,0);
    SDL_LockSurface(old);
    SDL_LockSurface(new);
    
    for (int y = 0; y < h0; y++)
    {
        for (int x = 0; x < w0; x++)
        {
            int x0 = (cosA * (x - x0c) + sinA * (y - y0c)) + x0c;
            int y0 = (-sinA * (x - x0c) + cosA * (y - y0c)) + y0c;
            
            if (x0 >= 0 && x0 < w0 && y0 >= 0 && y0 < h0)
            {
                Uint32 pix = getPixel(old, x0, y0);
                setPixel(new, x, y, pix);
            }
            else
                setPixel(new, x, y, SDL_MapRGB(old->format, 255, 255, 255));
        }
    }

    SDL_UnlockSurface(old);
    SDL_UnlockSurface(new);
    return new;
}
