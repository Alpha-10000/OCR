#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "functions.h"

void sortArray(Uint32 array[], int size)
{
  int i;
  for(i = 0; i < size; i++)
  {
    Uint32 temp = array[i];
    int j = i;
    while(j >= 0 && array[j-1] > temp)
    {
      array[j] = array[j-1];
      j -= 1;
    }
    array[j] = temp;
  }
}

void printAllocError(void)
{
  printf("Error : alloc fail");
  exit(EXIT_FAILURE);
}
