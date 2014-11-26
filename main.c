#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include "gui.h"

int main(int argc, char *argv[])
{
  /*-----Init GUI-----*/

  gtk_init(&argc, &argv);
  GtkWidget *mainWindow = guiInit();

  GtkWidget *textImage = gtk_image_new();
  GtkWidget *mainBox = initMainBox(mainWindow);
  initToolBar(mainBox, textImage);
  initMenu(mainBox, textImage);
  GtkWidget *mainZone = getMainZone(mainBox);
  gtk_box_pack_start(GTK_BOX(mainZone), textImage, FALSE, FALSE, 0);
  gtk_widget_show_all(mainWindow);

  /*------Init SDL------*/
  if (SDL_Init(SDL_INIT_VIDEO) == -1) //Starting SDL. If error
  {
    fprintf(stderr, "Error while initializing SDL : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  /*-----Principal code start-----*/
  SDL_Quit();
  gtk_main();
  return EXIT_SUCCESS;
}
