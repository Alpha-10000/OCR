#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "filters.h"


// Create vertical histograme of surface
int vertical_hist(SDL_Surface *surface, Uint16 *hist)
{
	// Needed for getPixel
	SDL_LockSurface(surface);
	int lines = 0;
	int onaLine = 0;
	// Y coordinate
	for(int j = 0; j < surface->h; j++)
	{
		hist[j] = 0;
		// X coordinate
		int i = 0;
		while(i < surface->w && hist[j] == 0)
		{
			// Get color at pos(i,j)
			Uint8 color;
			SDL_GetRGB(getPixel(surface, i, j), surface->format, 
					&color, &color, &color);
			// Is it black ?
			if (color == 0)
			{
				hist[j]++;
				if (!onaLine)
				{
					lines++;
					onaLine = 1;
				}
			}
			i++;
		}
		if (i == surface->w)
			onaLine = 0;
	}
	SDL_UnlockSurface(surface);
	return lines;
}

// Draw an horizontal red line from x0 to x1 at y0 on surface
void Draw_HLine(SDL_Surface *surface, int x0, int y0, int x1)
{
	SDL_Rect rect;
	rect.x = x0;
	rect.w = x1;
	rect.y = y0;
	rect.h = 1;
	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0));
}

void print_alloc_error()
{
	printf("Error : alloc fail");
	exit(EXIT_FAILURE);
}

// Return SDL_Rect array containing each text line
SDL_Rect *find_lines(SDL_Surface *surface)
{
	// Get surface vertical hist'
	Uint16 hist[surface->h];
	int size = vertical_hist(surface, hist);

	// Create lines array
	//size_t size = 20 * sizeof(SDL_Rect);
	SDL_Rect *lines = malloc(size * sizeof(SDL_Rect));
	//SDL_Rect *lines_realloc = NULL;

	if (lines == NULL){
		print_alloc_error();
	}

	// Bool -> on a line ? 
	// threshold at which we consider a line pixel as a part of a text line
	int onLine = 0, threshold = 1;

	size_t j = 0;
	for (int i = 0; i < surface->h; i++)
	{
		// Double array size when needed
		/*if (j >= size)
		{
			size *= 2;
			lines_realloc = realloc(lines, size);
			if (lines_realloc != NULL)
				lines = lines_realloc;
			else
				print_alloc_error();
		}*/

		if (!onLine)
		{
			if (hist[i] >= threshold)
			{
				// New line begins
				onLine = 1;
				lines[j].x = 0;
				lines[j].w = surface->w;
				lines[j].y = i;

				Draw_HLine(surface, 0, i, surface->w);
			}
		}

		else
		{
			if (hist[i] < threshold)
			{
				//Line reached end 
				onLine = 0;
				lines[j].h = i;
				Draw_HLine(surface, 0, i, surface->w);
				j++;
			}
		}
	}
	return lines;
}

// Used to check histogram
void print_tab(Uint16 *tab, int size)
{
	for(int i = 0; i < size; i++)
		printf("tab[%d] = %d\n", i, tab[i]);
}

// Create horizontal histograme of line
void horizontal_hist(SDL_Surface *surface, Uint16 *hist, SDL_Rect *line)
{
	//Needed for GetPixel
	SDL_LockSurface(surface);

	//X coordinate of a line
	for(int i = 0; i < line->w; i++)
	{
		hist[i] = 0;

		//Y coordinate of a line
		for(int j = 0; j < line->h; j++)
		{
			//Get color at pos(i,j)
			Uint8 color;
			SDL_GetRGB(getPixel(surface, i, j), surface->format,
					&color, &color, &color);
			//Is it black ?
			if (color == 0)
				hist[i]++;
		}
	}
	SDL_UnlockSurface(surface);
}

//Draw a vertical blue line from x0 to x1 at y0 on surface 
void Draw_Vline (SDL_Surface *surface, int x0, int y0, int y1)
{
	SDL_Rect rect;
	rect.x = x0;
	rect.h = y1;
	rect.y = y0;
	rect.w = 1;
	SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 0, 255));
}

/*
//Return array of matrix of struct Cell containing each character of a line
Cell *char_line(SDL_Surface *surface, SDL_Rect line)
{
Uint16 hist[line->w];
horizontal_hist(surface, hist);
return NULL;
}
 */
