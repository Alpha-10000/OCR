#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include "gui.h"
#include "functions.h"

int main(int argc, char *argv[])
{
  /*-----Init GUI-----*/

  gtk_init(&argc, &argv);
  GtkWidget *mainWindow = guiInit();
  gtk_window_maximize(GTK_WINDOW (mainWindow));
  gtk_window_set_title(GTK_WINDOW(mainWindow), "OCRulus");

  //Image and Text
  Zone *zone = malloc(sizeof(struct Zone*));
  zone->image = gtk_image_new();
  zone->text = gtk_text_view_new();

  //All Widgets
  GtkWidget *mainBox = initMainBox(mainWindow);
  initToolBar(mainBox, zone);
  initMenu(mainBox, zone);

  GtkWidget *mainZone = getMainZone(mainBox);
  gtk_box_pack_start(GTK_BOX(mainZone), zone->image, FALSE, FALSE, 0);
  gtk_box_pack_end(GTK_BOX(mainZone), zone->text, TRUE, TRUE, 0);
  gtk_widget_show_all(mainWindow);

  /*------Init SDL------*/
  if (SDL_Init(SDL_INIT_VIDEO) == -1) //Starting SDL. If error
  {
    fprintf(stderr, "Error while initializing SDL : %s\n", SDL_GetError());
    exit(EXIT_FAILURE);
  }

  SDL_Quit();
  gtk_main();
  return EXIT_SUCCESS;
}
