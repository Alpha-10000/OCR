#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

void vertical_hist(SDL_Surface *surface, Uint16 *hist);
void horizontal_hist(SDL_Surface *surface, Uint16 *hist, SDL_Rect *line);
void Draw_HLine(SDL_Surface *surface, int x0, int y0, int x1);
SDL_Rect *find_lines(SDL_Surface *surface);
void print_tab(Uint16 *tab, int size);

typedef struct Cell Cell;
struct Cell
{
	//Coordinates of the top-left corner of the rectangle
	int x;
	int y;
	//Dimensions
	int width;
	int height;
	//Matrixe of 0 and 1 for black and white pixels
	int char_matrix [][];
}
