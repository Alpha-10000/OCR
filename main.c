#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include "filters.h"
#include "network.h"
#include "detection.h"
#include "gui.h"

int main(int argc, char *argv[])
{
    /*-----Initialize Graphical User Interface-----*/
    /*gtk_init(&argc, &argv);
      guiInit();*/ //Annoying for debug
 
    /*-------SDL initialization-------*/
    network *test2 = initNetwork(3,2,2);

    int entry[4][2] = {{1,1}, {1,0},{0,1},{0,0}};
    int expected[4] = {0,1,1,0};

    learnNetwork(entry, expected, test2);

    computeOutput(entry[0],2,test2);
    printOutput(test2);
    computeOutput(entry[1],2,test2);
    printOutput(test2);
    computeOutput(entry[2],2,test2);
    printOutput(test2);
    computeOutput(entry[3],2,test2);
    printOutput(test2);

    freeNetwork(test2);

    if (SDL_Init(SDL_INIT_VIDEO) == -1) //Starting SDL. If error
    {
        fprintf(stderr, "Error while initializing SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE); //Exit the program
    }

    SDL_Surface *ecran = NULL; //The pointer representing the screen itself, the "background"
    ecran = SDL_SetVideoMode(700, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); //Opening the window by giving its size

    if (ecran == NULL) //If the opening failed
    {
        fprintf(stderr, "Impossible to load video mode : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("OCR", NULL); //Window title
    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 101, 148, 240)); //Color of the "background".
    SDL_EnableKeyRepeat(10, 10);

    /*--------Principal code start--------*/

    SDL_Surface *text = NULL;
    text = IMG_Load(argv[argc - 1]); //Loading the image we will work on
    if (text == NULL)
    {
        fprintf(stderr, "Give to the program an image as argument\n");
        exit(EXIT_FAILURE);
    }
    ecran = SDL_SetVideoMode(text->w, text->h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF); //Resize window to fit the image size
    SDL_Rect position; //Position of the image
    position.x = ecran->w / 2 - text->w / 2;
    position.y = ecran->h / 2 - text->h / 2;
    SDL_BlitSurface(text, NULL, ecran, &position); //Display the image

    greyScale(text);
    //noiseRemove(text);
    binarize(text);
    
    int nbLines;
    Block *blocks = find_blocks(text, &nbLines);
    find_chars(text, blocks, nbLines);
    free(blocks);

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
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_UP:
                        position.y += 5;
                        break;
                    case SDLK_DOWN:
                        position.y -= 5;
                        break;
                    default:
                        break;
                }
            case SDL_MOUSEBUTTONUP:
                switch(event.button.button)
                {
                    case SDL_BUTTON_WHEELUP:
                        position.y += 10;
                        break;
                    case SDL_BUTTON_WHEELDOWN:
                        position.y -= 10;
                        break;
                } 
            default:
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
