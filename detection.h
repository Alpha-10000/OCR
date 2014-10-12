#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void vertical_hist(SDL_Surface *surface, Uint16 *hist);
void Draw_HLine(SDL_Surface *surface, int x0, int y0, int x1);
SDL_Rect *find_lines(SDL_Surface *surface);
void print_tab(Uint16 *tab, int size);
