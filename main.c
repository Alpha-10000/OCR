#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include "filters.h"
#include "network.h"
#include "detection.h"
#include "gui.h"
#include "functions.h"

int main(int argc, char *argv[])
{
  /*-----Initialize Graphical User Interface-----*/
  gtk_init(&argc, &argv);
  GtkWidget *mainWindow = guiInit();
  GtkWidget *mainBox = initMainBox(mainWindow);
  /*-------SDL initialization-------*/
  /*
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
  */
  if (SDL_Init(SDL_INIT_VIDEO) == -1) //Starting SDL. If error
  {
    fprintf(stderr, "Error while initializing SDL : %s\n", SDL_GetError());
    exit(EXIT_FAILURE); //Exit the program
  }

  /*SDL_Surface *ecran = NULL; //The pointer representing the screen.
  ecran = SDL_SetVideoMode(700, 700, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

  if (ecran == NULL) //If the opening failed
  {
    fprintf(stderr, "Impossible to load video mode : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_WM_SetCaption("OCR", NULL); //Window title
  SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
  SDL_EnableKeyRepeat(10, 10);*/

   /*--------Principal code start--------*/

  SDL_Surface *text = NULL;
  text = IMG_Load(argv[argc - 1]); //Loading the image we will work on
  if (text == NULL)
  {
    fprintf(stderr, "Give to the program an image as argument\n");
    exit(EXIT_FAILURE);
  }

  greyScale(text);
  ///noiseRemove(text);
  binarize(text);

  int nbLines;
  Block *blocks = findBlocks(text, &nbLines);
  //print_blocks(blocks, nbLines);
  findChars(text, blocks, nbLines);
  //drawLinesChars(text, blocks, nbLines);
  ///SDL_Surface *resized = NULL;
  ///resized = resizeChars(text, blocks, nbLines);
  ///ecran = SDL_SetVideoMode(text->w, text->h, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
  freeBlocks(blocks, nbLines);

  /*------Main GTK loop-------*/
  GdkPixbuf *pixBuf = loadPixBuf(text);
  GtkWidget *textImage = gtk_image_new_from_pixbuf(pixBuf);

  gtk_box_pack_start(GTK_BOX(mainBox), textImage, FALSE, FALSE, 0);
  gtk_widget_show_all(mainWindow);
   /*--------Principal code end-------*/

  gtk_main();

  SDL_FreeSurface(text); //Free surface's memory
  SDL_Quit(); //Exit
  return EXIT_SUCCESS;
}
