#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "filters.h"
#include "detection.h"
#include "network.h"

void freeBlocks(Block *b, int nbLines)
{
  for (int i = 0; i < nbLines; i++)
    free(b[i].chars);
  free(b);
}

// Create vertical histograme of surface
void verticalHist(SDL_Surface *surface, Uint16 *hist, int *nbLines)
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

// Return Block array containing each text line
Block *findBlocks(SDL_Surface *surface, int *nbLines)
{
  // Get surface vertical hist'
  Uint16 hist[surface->h];
  verticalHist(surface, hist, nbLines);
  // Create lines array
  Block *blocks = malloc(*nbLines * sizeof(Block));
  if (blocks == NULL){
    printAllocError();
  }
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
     blocks[j].line.h = i - blocks[j].line.y;
     j++;
   }
 }
}
return blocks;
}

// Used to check histogram
void printTab(Uint16 *tab, int size)
{
  for(int i = 0; i < size; i++)
    printf("tab[%d] = %d\n", i, tab[i]);
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

// Create horizontal histogram of line
int horizontalHist(SDL_Surface *surface, Uint16 *hist, Block *block)
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

void findChars(SDL_Surface *surface, Block *blocks, int nbLines)
{
  for(int cur_Line = 0; cur_Line < nbLines; cur_Line++)
  {
      // Vertical hist for this line
    Uint16 hist[blocks[cur_Line].line.w];
    blocks[cur_Line].nbChars = horizontalHist(surface, hist,
     &blocks[cur_Line]);

      // Testing
      //print_tab(hist, blocks[cur_Line].line.w);
      //printf("Line %d nb chars : %d\n", cur_Line, blocks[cur_Line].nbChars);
      // Create chars array
    blocks[cur_Line].chars = malloc(blocks[cur_Line].nbChars *
      sizeof(SDL_Rect));

      // Bool -> on a char ?
      // threshold at which we consider a line pixel as a part of a char
    int onChar = 0, threshold = 1, cur_Char = 0;
    for (int k = 0; k < blocks[cur_Line].line.w; k++)
    {
      if (!onChar)
      {
        if (hist[k] >= threshold)
        {
	         // New line begins
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
	    //Line reached end
         onChar = 0;
         blocks[cur_Line].chars[cur_Char].w =
         k - blocks[cur_Line].chars[cur_Char].x;
         centerChar(surface, &blocks[cur_Line].chars[cur_Char]);
         cur_Char++;
       }
     }
   }
 }
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
    maxCharsPerLine*resolution,
    nbLines*resolution+1,
    surface->format->BitsPerPixel,0,0,0,0);

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
      for (int x = 0; x <= resolution; x++)
      {
        for (int y = 0; y <= resolution; y++)
        {
          Uint32 pixel = getPixel(surface,
          blocks[nbLine].chars[charn].x + x*blocks[nbLine].chars[charn].w/resolution,
          blocks[nbLine].chars[charn].y + y*blocks[nbLine].chars[charn].h/resolution);

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
