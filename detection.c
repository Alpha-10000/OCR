#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "filters.h"
#include "detection.h"
#include "network.h"
#include "functions.h"

// Create vertical histograme of surface
void verticalHist(SDL_Surface *surface, int *hist, int *nbLines)
{
  // Needed for getPixel
  SDL_LockSurface(surface);
  *nbLines = 0;
  int onLine = 0;
  // Y coordinate
  for(int j = 0; j < surface->h; j++)
  {
    hist[j] = 0;
    // X coordinate
    int i;
    for (i = 0; i < surface->w && !hist[j]; i++)
    {
      // Get color at pos(i,j)
      Uint8 color;
      SDL_GetRGB(getPixel(surface, i, j), surface->format,
		 &color, &color, &color);
      // Is it black ?
      if (!color)
      {
	hist[j]++;

	if (!onLine)
	{
	  (*nbLines)++;
	  onLine = 1;
	}
      }
    }

    if (i == surface->w)
      onLine = 0;
  }
  SDL_UnlockSurface(surface);
}

// Create horizontal histogram of line
int horizontalHist(SDL_Surface *surface, int *hist, Block *block)
{
  //Needed for GetPixel
  SDL_LockSurface(surface);

  int nbChars = 0, onChar = 0;
  //X coordinate of a line
  for(int i = 0; i < block->line.w; i++)
  {
    hist[i] = 0;
    //Y coordinate of a line
    int j;
    for(j = 0; j < block->line.h && !hist[i]; j++)
    {
      //Get color at pos(x+i, y+j)
      Uint8 color;
      SDL_GetRGB(getPixel(surface, block->line.x + i, block->line.y + j),
		 surface->format, &color, &color, &color);
      //Is it black ?
      if (!color)
      {
	hist[i]++;
	if (!onChar)
	{
	  nbChars++;
	  onChar = 1;
	}
      }
    }
    if (j == block->line.h)
      onChar = 0;
  }

  SDL_UnlockSurface(surface);
  return nbChars;
}

// Return Block array containing each text line
Block *findBlocks(SDL_Surface *surface, int *nbLines)
{
  // Get surface vertical hist'
  int hist[surface->h];
  verticalHist(surface, hist, nbLines);

  // Create lines array
  Block *blocks = malloc(*nbLines * sizeof(Block));
  initBlocks(blocks, *nbLines);

  // Bool -> on a line ?
  // threshold at which we consider a line pixel as a part of a text line
  int onLine = 0, threshold = 1, j = 0;
  for (int i = 0; i < surface->h; i++)
  {
    if (!onLine)
    {
      if (hist[i] >= threshold)
      {
	// New line begins
	onLine = 1;
	blocks[j].line.x = 0;
	blocks[j].line.w = surface->w;
	blocks[j].line.y = i;
      }
    }
    else
    {
      if (hist[i] < threshold)
      {
	//Line reached end
	onLine = 0;
	blocks[j].line.h = i - blocks[j].line.y + 1;
	j++;
      }
    }
  }
  return blocks;
}

void findChars(SDL_Surface *surface, Block *blocks, int nbLines)
{
  for(int cur_Line = 0; cur_Line < nbLines; cur_Line++)
  {
    // Vertical hist for this line
    int hist[blocks[cur_Line].line.w];
    blocks[cur_Line].nbChars = horizontalHist(surface, hist,
					      &blocks[cur_Line]);

    // Testing
    //printf("Line %d nb chars : %d\n", cur_Line, blocks[cur_Line].nbChars);

    // Create chars array
    blocks[cur_Line].chars = malloc(blocks[cur_Line].nbChars *
				    sizeof(SDL_Rect));
    blocks[cur_Line].spaces = malloc(blocks[cur_Line].nbChars *
				     sizeof(int));
    initChars(blocks, cur_Line);

    // Bool -> on a char ?
    // threshold at which we consider a line pixel as a part of a char
    int onChar = 0, threshold = 1, cur_Char = 0;
    for (int k = 0; k < blocks[cur_Line].line.w; k++)
    {
      if (!onChar)
      {
	if (hist[k] >= threshold)
	{
	  // New char begins
	  onChar = 1;
	  blocks[cur_Line].chars[cur_Char].x = k;
	  blocks[cur_Line].chars[cur_Char].y = blocks[cur_Line].line.y;
	  blocks[cur_Line].chars[cur_Char].h = blocks[cur_Line].line.h;
	}
      }
      else
      {
	if (hist[k] < threshold)
	{
	  //Char reached end
	  onChar = 0;
	  blocks[cur_Line].chars[cur_Char].w =
	    k - blocks[cur_Line].chars[cur_Char].x;
	  centerChar(surface, &blocks[cur_Line].chars[cur_Char]);
	  cur_Char++;
	}
      }
    }
    // Resize line at correct left and right borders
    blocks[cur_Line].line.x = blocks[cur_Line].chars[0].x;
    blocks[cur_Line].line.w = blocks[cur_Line].chars[cur_Char - 1].x +
      blocks[cur_Line].chars[cur_Char - 1].w -
      blocks[cur_Line].line.x;

    detectSpaces(blocks, cur_Line);
    //Double char
    //doubleChars(blocks, cur_Line);
  }
}

void centerChar(SDL_Surface *surface, SDL_Rect *rect)
{
  SDL_LockSurface(surface);
  int result = 1;
  int counter = -1;
  Uint32 pixel;
  Uint8 grey;
  for (int y = 0; y < rect->h && result; y++)
  {
    for (int x = 0; x < rect->w && result; x++)
    {
      pixel = getPixel(surface, rect->x + x, rect->y + y);
      SDL_GetRGB(pixel, surface->format, &grey, &grey, &grey);
      if (grey == 0)
	result = 0;
    }
    counter++;
  }
  rect->y = rect->y + counter;
  rect->h = rect->h - counter;
  counter = -1;
  result = 1;
  for (int y = rect->h; y >= 0 && result; y--)
  {
    for (int x = 0; x < rect->w && result; x++)
    {
      pixel = getPixel(surface, rect->x + x, rect->y + y);
      SDL_GetRGB(pixel, surface->format, &grey, &grey, &grey);
      if (grey == 0)
	result = 0;
    }
    counter++;
  }
  rect->h = rect->h - counter;
  SDL_UnlockSurface(surface);
  rect->x--;
  rect->y--;
  rect->w += 2;
  rect->h += 2;
}

SDL_Surface* resizeChars(SDL_Surface *surface, Block *blocks, int nbLines)
{
  int resolution = NN_RESOLUTION;

  int nbChars = 0;
  int maxCharsPerLine = blocks[0].nbChars;
  for (int i = 0; i < nbLines; i++)
  {
    nbChars+=blocks[i].nbChars;
    if(blocks[i].nbChars > maxCharsPerLine)
      maxCharsPerLine = blocks[i].nbChars;
  }
  SDL_Surface *copy = SDL_CreateRGBSurface(0,
					   maxCharsPerLine*resolution+1,
					   nbLines*resolution,
					   surface->format->BitsPerPixel,
					   0,0,0,0);

  SDL_LockSurface(surface);
  SDL_LockSurface(copy);

  for (int i = 0; i < copy->w; i++)
  {
    for (int j = 0; j < copy->h; j++)
    {
      Uint32 pixel = getPixel(surface,0,0);
      setPixel(copy, i, j, pixel);
    }
  }

  for (int nbLine = 0; nbLine < nbLines; nbLine++)
  {
    for (int charn = 0; charn < blocks[nbLine].nbChars; charn++)
    {
      for (int x = 0; x < resolution; x++)
      {
	for (int y = 0; y < resolution; y++)
	{
	  Uint32 pixel = getPixel(surface,
				  blocks[nbLine].chars[charn].x +
				  x * blocks[nbLine].chars[charn].w/resolution,
				  blocks[nbLine].chars[charn].y +
				  y * blocks[nbLine].chars[charn].h/resolution);

	  setPixel(copy,
		   charn*resolution + x,
		   nbLine*resolution + y,
		   pixel);
	}
      }
    }
  }
  SDL_UnlockSurface(copy);
  SDL_UnlockSurface(surface);
  return copy;
}

void detectSpaces(Block *b, int l)
{
  int spaceTresh = medianCharSize(b, l) / 2;
  for (int c = 0; c < b[l].nbChars - 1; c++)
  {
    int space = b[l].chars[c + 1].x - b[l].chars[c].x - b[l].chars[c].w;
    if (space >= spaceTresh)
      b[l].spaces[c] = space / spaceTresh;
  }
}

int medianCharSize(Block *b, int line)
{
  int total = 0;
  for (int i = 0; i < b[line].nbChars; i++)
    total+= b[line].chars[i].w;

  return total/b[line].nbChars;
}
void doubleChars(Block *b, int line)
{
  int medianSize = medianCharSize(b, line);

  for (int i = 0; i < b[line].nbChars; i++)
  {
    if (b[line].chars[i].w > medianSize)
    {
      int nb = b[line].chars[i].w / medianSize;
      if (nb > 1)
      {
	int oldNbChars = b[line].nbChars;
	SDL_Rect old = b[line].chars[i];
	b[line].nbChars += nb - 1;
	b[line].chars = realloc(b[line].chars, (b[line].nbChars
						* sizeof(SDL_Rect)));

	for (int j = oldNbChars - 1; j > i + nb - 1; j--)
	  b[line].chars[j + nb - 1] = b[line].chars[j];

	for (int j = 0; j < nb; j++)
	{
	  b[line].chars[j + i].x = old.x + j / nb * old.w;
	  b[line].chars[j + i].w = old.w / nb;
	  b[line].chars[j + i].y = old.y;
	  b[line].chars[j + i].h = old.h;
	}
      }
    }
  }
}

// Drawing functions
// Draw an horizontal red line from x of length w at y on surface
void drawHLine(SDL_Surface *surface, int x, int y, int w)
{
  SDL_Rect rect;
  rect.x = x;
  rect.w = w;
  rect.y = y;
  rect.h = 1;
  SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 232, 0, 52));
}

//Draw a vertical blue line from y of length h at x on surface
void drawVline (SDL_Surface *surface, int x, int y, int h)
{
  SDL_Rect rect;
  rect.x = x;
  rect.h = h;
  rect.y = y;
  rect.w = 1;
  SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 5, 199, 206));
}

void drawLinesChars(SDL_Surface *surface, Block *blocks, int nbLines)
{
  for(int i = 0; i < nbLines; i++)
  {
    SDL_Rect line = blocks[i].line;
    drawHLine(surface, line.x, line.y, line.w);
    drawHLine(surface, line.x, line.y + line.h, line.w);

    for(int j = 0; j < blocks[i].nbChars; j++)
    {
      SDL_Rect ch = blocks[i].chars[j];
      drawVline(surface, ch.x, ch.y, ch.h);
      drawVline(surface, ch.x + ch.w, ch.y, ch.h);
      drawHLine(surface, ch.x, ch.y, ch.w);
      drawHLine(surface, ch.x, ch.y + ch.h, ch.w);
    }
  }
}

// Init functions
// Init array with value
void initArray(int *array, int size, int value)
{
  for (int i = 0; i < size; i++)
    array[i] = value;
}

void initBlocks(Block *b, int size)
{
  for (int i = 0; i < size; i++)
  {
    b[i].line.x = 0;
    b[i].line.y = 0;
    b[i].line.w = 0;
    b[i].line.h = 0;
    b[i].nbChars = 0;
  }
}

void initChars(Block *b, int line)
{
  for (int i = 0; i < b[line].nbChars; i++)
  {
    b[line].spaces[i] = 0;
    b[line].chars[i].x = 0;
    b[line].chars[i].y = 0;
    b[line].chars[i].w = 0;
    b[line].chars[i].h = 0;
  }
}

// Test functions
// Used to check histogram
void printTab(int *array, int size)
{
  for(int i = 0; i < size; i++)
    printf("array[%d] = %d\n", i, array[i]);
}

// Check find blocks
void printBlocks(Block *blocks, int size)
{
  for(int i = 0; i < size; i++)
  {
    printf("line %d : x = %d; w = %d; y = %d; h = %d\n", i,
	   blocks[i].line.x, blocks[i].line.w,
	   blocks[i].line.y, blocks[i].line.h);
  }
}

void printSpaces(Block *b, int size)
{
  for(int i = 0; i < size; i++)
  {
    printf("Line %d\n", i);
    printTab(b[i].spaces, b[i].nbChars);
  }
}

void printMedianCharSize(Block *b, int size)
{
  for (int i = 0; i < size; i++)
    printf("Median char size on line %d = %d\n", i,
	   medianCharSize(b, i));
}

void freeBlocks(Block *b, int nbLines)
{
  for (int i = 0; i < nbLines; i++)
  {
    free(b[i].chars);
    free(b[i].spaces);
  }
  free(b);
}
