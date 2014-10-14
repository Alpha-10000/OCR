#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "gui.h"

void cb_quit (GtkWidget *p_widget, gpointer user_data)
{
    gtk_main_quit();
    (void)p_widget;
    (void)user_data;
}
