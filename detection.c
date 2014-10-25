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

// Draw an horizontal red line from x of length w at y on surface
void Draw_HLine(SDL_Surface *surface, int x, int y, int w)
{
    SDL_Rect rect;
    rect.x = x;
    rect.w = w;
    rect.y = y;
    rect.h = 1;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 255, 0, 0));
}

void print_alloc_error()
{
    printf("Error : alloc fail");
    exit(EXIT_FAILURE);
}

// Return Block array containing each text line
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

                //Draw_HLine(surface, 0, i, surface->w);
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

                //Draw_HLine(surface, 0, i, surface->w);
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

// Check find blocks
void print_blocks(Block *blocks, int size)
{
    for(int i = 0; i < size; i++)
    {
        printf("line %d : x = %d; w = %d; y = %d; h = %d\n", i,
                blocks[i].line.x, blocks[i].line.w,
                blocks[i].line.y, blocks[i].line.h);
    }
}

// Create horizontal histogram of line
int horizontal_hist(SDL_Surface *surface, Uint16 *hist, Block *block)
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

//Draw a vertical green line from y of length h at x on surface 
void Draw_Vline (SDL_Surface *surface, int x, int y, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.h = h;
    rect.y = y;
    rect.w = 1;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0, 255, 0));
}

void find_chars(SDL_Surface *surface, Block *blocks, int nbLines)
{
    for(int cur_Line = 0; cur_Line < nbLines; cur_Line++)
    {
        // Vertical hist for this line
        Uint16 hist[blocks[cur_Line].line.w];
        blocks[cur_Line].nbChars = horizontal_hist(surface, hist, 
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
                    blocks[cur_Line].chars[cur_Char].y = 
                        blocks[cur_Line].line.y;
                    blocks[cur_Line].chars[cur_Char].h =
                        blocks[cur_Line].line.h;

                    Draw_Vline(surface, k, blocks[cur_Line].line.y,
                            blocks[cur_Line].line.h);
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
                    cur_Char++;

                    Draw_Vline(surface, k, blocks[cur_Line].line.y,
                            blocks[cur_Line].line.h);
                }
            }
        }
    }
}
