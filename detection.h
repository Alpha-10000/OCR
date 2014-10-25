#ifndef DETECTION
#define DETECTION

#include <SDL/SDL.h>

typedef struct Block Block;
struct Block
{
    //For the line
    SDL_Rect line;
    int nbChars;
    SDL_Rect *chars;
};

void vertical_hist(SDL_Surface *surface, Uint16 *hist, int *nbLines);

int horizontal_hist(SDL_Surface *surface, Uint16 *hist, Block *block);

Block *find_blocks(SDL_Surface *surface, int* nbLines);

void find_chars(SDL_Surface *surface, Block *blocks, int nbLines);

void print_tab(Uint16 *tab, int size);

void print_blocks(Block *blocks, int size);

void draw_lines_chars(SDL_Surface *surface, Block *blocks, int nbLines);

#endif
