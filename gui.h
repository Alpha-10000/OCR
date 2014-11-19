#ifndef GUI
#define GUI
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

void cb_quit (GtkWidget *p_widget, gpointer user_data);
GdkPixbuf *loadPixBuf(SDL_Surface *surface);
GtkWidget *guiInit(void);
GtkWidget *initMainBox(GtkWidget *window);
#endif
