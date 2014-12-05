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
  int *spaces;
};

void freeBlocks(Block *b, int nbLines);

void verticalHist(SDL_Surface *surface, int *hist, int *nbLines);

int horizontalHist(SDL_Surface *surface, int *hist, Block *block);

void drawHLine(SDL_Surface *surface, int x, int y, int w);

Block *findBlocks(SDL_Surface *surface, int *nbLines);

void findChars(SDL_Surface *surface, Block *blocks, int nbLines);

void prinTab(int *array, int size);

void printBlocks(Block *blocks, int size);

void drawVline (SDL_Surface *surface, int x, int y, int h);

void drawLinesChars(SDL_Surface *surface, Block *blocks, int nbLines);

void centerChar(SDL_Surface *surface, SDL_Rect *rect);

SDL_Surface* resizeChars(SDL_Surface *surface, Block *blocks, int nbLines);

void detectSpaces(Block *b, int nbLine);

void initArray(int *array, int size, int value);

void initBlocks(Block *b, int size);

void initChars(Block *b, int line);

void printSpaces(Block *b, int size);

#endif
