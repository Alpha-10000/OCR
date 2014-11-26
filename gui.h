#ifndef GUI
#define GUI
#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "functions.h"

void cb_quit(GtkWidget *widget, gpointer data);
GdkPixbuf *loadPixBuf(SDL_Surface *surface);
void cb_open(GtkWidget *widget, gpointer data);
GtkWidget *guiInit(void);
GtkWidget *initMainBox(GtkWidget *window);
void initMenu(GtkWidget* box, Zone *zone);
void initToolBar(GtkWidget *box, Zone *zone);
GtkWidget *getMainZone(GtkWidget *box);
#endif
