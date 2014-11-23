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
  mainBox = gtk_hbox_new(FALSE, 0);
  gtk_container_add(GTK_CONTAINER(window), mainBox);

  /*-----Menu----*/
  /*Menu Bar*/
  GtkWidget *menuBar = NULL;
  menuBar = gtk_menu_bar_new();

  /*Menu 1*/
  GtkWidget *menu = NULL;
  menu = gtk_menu_new();
  /*Menu items*/
  GtkWidget *menuItem = NULL;
  menuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  menuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  menuItem = gtk_separator_menu_item_new();
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  menuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
  g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(cb_quit), NULL);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  /*Submenu*/
  menuItem = gtk_menu_item_new_with_label("File");
  /*Link menu items*/
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  /*Add to menu bar*/
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);

  /*Menu 2*/
  menu = gtk_menu_new();
  /*Menu items*/
  menuItem = gtk_menu_item_new_with_label("Help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  menuItem = gtk_menu_item_new_with_label("About");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);
  /*Subemenu*/
  menuItem = gtk_menu_item_new_with_label("?");
  /*Link menu items*/
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  /*Add to menuBar*/
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);

  /*Add bar to window*/
  gtk_box_pack_start(GTK_BOX(mainBox), menuBar, FALSE, FALSE, 0);

  /*----Text zone----*/
  GtkWidget *textView = NULL;
  textView = gtk_text_view_new();
  gtk_box_pack_end(GTK_BOX(mainBox), textView, TRUE, TRUE, 0);
  return mainBox;
}
