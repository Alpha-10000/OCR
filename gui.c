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

GdkPixbuf *loadPixBuf(SDL_Surface *surface)
{
  SDL_SaveBMP(surface, "data.bmp");
  GError ** error = NULL;
  GdkPixbuf *pixBuf = NULL;
  pixBuf = gdk_pixbuf_new_from_file("data.bmp", error);
  return pixBuf;
}

GtkWidget *guiInit(void)
{
  /*------Main Window-------*/
  GtkWidget *mainWindow = NULL;
  mainWindow = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  g_signal_connect(G_OBJECT(mainWindow), "destroy", G_CALLBACK(cb_quit), NULL);
  return mainWindow;
}

GtkWidget *initMainBox(GtkWidget *window)
{
  GtkWidget *mainBox = NULL;
  mainBox = gtk_vbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), mainBox);

  GtkWidget *quitButton = NULL;
  quitButton = gtk_button_new_from_stock(GTK_STOCK_QUIT);
  g_signal_connect(G_OBJECT(quitButton), "clicked", G_CALLBACK(cb_quit), NULL);
  gtk_box_pack_start(GTK_BOX(mainBox), quitButton, FALSE, FALSE, 0);
  return mainBox;
}
