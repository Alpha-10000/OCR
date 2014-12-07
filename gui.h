#ifndef GUI
#define GUI

#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <wchar.h>
#include "functions.h"

void cb_quit(GtkWidget *widget, gpointer data);
GdkPixbuf *loadPixBuf(SDL_Surface *surface);
void cb_open(GtkWidget *widget, gpointer data);
void cb_save(GtkWidget *widget, gpointer data);
void cb_lang(GtkWidget *widget, gpointer data);
void cb_zoomi(GtkWidget *widget, gpointer data);
void cb_zoomo(GtkWidget *widget, gpointer data);
void displayOutput(wchar_t *output, Zone *zone);
GtkWidget *guiInit(void);
GtkWidget *initMainBox(GtkWidget *window);
void initMenu(GtkWidget* box, Zone *zone);
void initToolBar(GtkWidget *box, Zone *zone);
void setMainZone(GtkWidget *box, Zone *zone);

#endif
