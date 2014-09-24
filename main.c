#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include "filters.h"
int main(int argc, char *argv[])
{
    /*-----------SDL initialization---------*/

    if (SDL_Init(SDL_INIT_VIDEO) == -1) //Starting SDL. If error :
    {
	fprintf(stderr, "Error while initializing SDL : %s\n", SDL_GetError());
	exit(EXIT_FAILURE); //Exit the program
    }

    SDL_Surface *ecran = NULL; //The pointer representing the screen itself, the "background"
    ecran = SDL_SetVideoMode(640, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_RESIZABLE); //Opening the window by giving its size

    if (ecran == NULL) //If the opening failed :
    {
	fprintf(stderr, "Impossible to load video mode : %s\n", SDL_GetError());
	exit(EXIT_FAILURE);
    }
    
    SDL_WM_SetCaption("OCR", NULL); //Window title
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 101, 148, 240)); //Color of the "background".
    
    /*--------Principal code start--------*/
    
    SDL_Surface *text = NULL;
    text = IMG_Load(argv[argc - 1]); //Loading the image we will work on
    
    SDL_Rect position; //Position of the image
    position.x = ecran->w / 2 - text->w / 2;
    position.y = ecran->h / 2 - text->h / 2;
    SDL_BlitSurface(text, NULL, ecran, &position); //Display the image

    greyScale(text);
    int continuer = 1;
    SDL_Event event;
    while (continuer) //Update loop
    {
	SDL_WaitEvent(&event);
	switch(event.type)
	{
	    case SDL_QUIT:
	        continuer = 0;
	        break;
	}
    
	SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 101, 148, 240)); //Clear the screen
	SDL_BlitSurface(text, NULL, ecran, &position); //Redraw each surface after update
	SDL_Flip(ecran); //Display new state of all elements after update
    }
    
    /*--------Principal code end-------*/

    SDL_FreeSurface(text); //Free surface's memory
    SDL_Quit(); //Exit SDL

    return EXIT_SUCCESS;
}
