#ifndef GUI
#define GUI
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

void cb_quit (GtkWidget *p_widget, gpointer user_data);
GdkPixmap *loadPixMap(GdkWindow *window, SDL_Surface *surface, Uint8 **matrix);
GtkWidget *guiInit(void);

#endif
