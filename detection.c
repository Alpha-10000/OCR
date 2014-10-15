#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "filters.h"
#include "detection.h"

// Create vertical histograme of surface
void vertical_hist(SDL_Surface *surface, Uint16 *hist, int *nbLines)
{
    // Needed for getPixel
    SDL_LockSurface(surface);
    *nbLines = 0;
    int onaLine = 0;
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
                if (!onaLine)
                {
                    (*nbLines)++;
                    onaLine = 1;
                }
            }
        }
        if (i == surface->w)
            onaLine = 0;
    }
    SDL_UnlockSurface(surface);
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
Block *find_blocks(SDL_Surface *surface, int *nbLines)
{
    // Get surface vertical hist'
    Uint16 hist[surface->h];
    vertical_hist(surface, hist, nbLines);

    // Create lines array
    Block *blocks = malloc(*nbLines * sizeof(Block));

    if (blocks == NULL){
        print_alloc_error();
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

                Draw_HLine(surface, 0, i, surface->w);
            }
        }

        else
        {
            if (hist[i] < threshold)
            {
                //Line reached end 
                onLine = 0;
                blocks[j].line.h = i;
                Draw_HLine(surface, 0, i, surface->w);
                j++;
            }
        }
    }
    return blocks;
}

// Used to check histogram
void print_tab(Uint16 *tab, int size)
{
    for(int i = 0; i < size; i++)
        printf("tab[%d] = %d\n", i, tab[i]);
}

// Create horizontal histograme of line
int horizontal_hist(SDL_Surface *surface, Uint16 *hist, Block *block)
{
    //Needed for GetPixel
    SDL_LockSurface(surface);
    int columns = 0;
    int onaCol = 0;
    //X coordinate of a line
    for(int i = 0; i < block->line.w; i++)
    {
        hist[i] = 0;
        //Y coordinate of a line
        int j = 0;
        for(j = block->line.y; j < block->line.h && hist[i] == 0; j++)
        {
            //Get color at pos(i,j)
            Uint8 color;
            SDL_GetRGB(getPixel(surface, i, j), surface->format,
                    &color, &color, &color);
            //Is it black ?
            if (color == 0)
            {
                hist[i]++;
                if (!onaCol)
                {
                    columns++;
                    onaCol = 1;
                }
            }
        }
        if (j == block->line.h)
            onaCol = 0;

    }
    SDL_UnlockSurface(surface);
    return columns;
}

//Draw a vertical blue line from x0 to x1 at y0 on surface 
void Draw_Vline (SDL_Surface *surface, int x0, int y0, int y1)
{
    SDL_Rect rect;
    rect.x = x0;
    rect.h = y1;
    rect.y = y0;
    rect.w = 1;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 255, 0));
}

void find_chars(SDL_Surface *surface, Block *blocks, int nbLines)
{
    for(int currentLine = 0; currentLine < nbLines; currentLine++)
    {
        Uint16 hist[blocks[currentLine].line.w];
        blocks[currentLine].nbChars = horizontal_hist(surface, hist, &blocks[currentLine]);
        //printf("%d\n", blocks[currentLine].nbChars);
        blocks[currentLine].characters = malloc(blocks[currentLine].nbChars * sizeof(SDL_Rect));
        // Bool -> on a line ? 
        // threshold at which we consider a line pixel as a part of a text line
        int onChar = 0, threshold = 1, currentChar = 0;
        for (int k = 0; k < blocks[currentLine].line.w; k++)
        {
            if (!onChar)
            {
                if (hist[k] >= threshold)
                {
                    // New line begins
                    onChar = 1;
                    blocks[currentLine].characters[currentChar].x = k;
                    blocks[currentLine].characters[currentChar].y = blocks[currentLine].line.y;
                    Draw_Vline(surface, k, blocks[currentLine].line.y,
                           blocks[currentLine].line.h - blocks[currentLine].line.y);
                }
            }

            else
            {
                if (hist[k] < threshold)
                {
                    //Line reached end 
                    onChar = 0;
                    blocks[currentLine].characters[currentChar].w =
                       k - blocks[currentLine].characters[currentChar].x; 
                    blocks[currentLine].characters[currentChar].h =
                        blocks[currentLine].line.h;
                    Draw_Vline(surface, k, blocks[currentLine].line.y,
                            blocks[currentLine].line.h - blocks[currentLine].line.y);
                    currentChar++;
                }
            }
        }
    }
}

