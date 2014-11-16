#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "filters.h"
#include "functions.h"
#include "gui.h"

void cb_quit (GtkWidget *p_widget, gpointer user_data)
{
  gtk_main_quit();
  (void)p_widget;
  (void)user_data;
}


GdkPixmap *loadPixMap(GdkWindow *window, SDL_Surface *surface, Uint8 **matrix)
{
  getImageData(surface, matrix);
  GdkPixmap *pixMap = gdk_pixmap_create_from_xpm(window, NULL,
						  NULL, "data.xpm");
  return pixMap;
}

GtkWidget *guiInit(void)
{
  /*------Main Window-------*/
  GtkWidget *mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(cb_quit), NULL);

  //Quit button
  GtkWidget *quitButton = NULL;
  quitButton = gtk_button_new_from_stock(GTK_STOCK_QUIT);
  gtk_container_add(GTK_CONTAINER(mainWindow), quitButton);
  g_signal_connect(G_OBJECT(quitButton), "clicked", G_CALLBACK(cb_quit), NULL);
  return mainWindow;
}
