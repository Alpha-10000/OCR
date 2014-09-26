#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

Uint32 getPixel(SDL_Surface *surface, int x, int y);
void setPixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void sortArray(Uint32 array[], int size);
void noiseRemove(SDL_Surface *surface);
void greyScale(SDL_Surface *surface);
void binarize(SDL_Surface *surface);
