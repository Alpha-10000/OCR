#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

typedef struct Block Block;
struct Block
{
    //For the line
    SDL_Rect line;
    int nbChars;
    SDL_Rect *characters;
};

void vertical_hist(SDL_Surface *surface, Uint16 *hist, int *nbLines);
int horizontal_hist(SDL_Surface *surface, Uint16 *hist, Block *block);
void Draw_HLine(SDL_Surface *surface, int x0, int y0, int x1);
Block *find_blocks(SDL_Surface *surface, int* nbLines);
void find_chars(SDL_Surface *surface, Block *blocks, int nbLines);
void print_tab(Uint16 *tab, int size);
