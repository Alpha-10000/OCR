#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "filters.h"
#include "functions.h"
#include "gui.h"

void cb_quit (GtkWidget *widget, gpointer data)
{
  gtk_main_quit();
  (void)widget;
  (void)data;
}

GdkPixbuf *loadPixBuf(SDL_Surface *surface)
{
  SDL_SaveBMP(surface, "data.bmp");
  GError **error = NULL;
  GdkPixbuf *pixBuf = NULL;
  pixBuf = gdk_pixbuf_new_from_file("data.bmp", error);
  GdkPixbuf *miniBuf = NULL;
  miniBuf = gdk_pixbuf_scale_simple(pixBuf,
				    gdk_pixbuf_get_width(pixBuf)/2,
				    gdk_pixbuf_get_height(pixBuf)/2,
				    GDK_INTERP_NEAREST);
  return miniBuf;
}

void cb_open(GtkWidget *widget, gpointer data)
{
  Zone *zone = (Zone*)data;
  GtkWidget *image = NULL;
  image = GTK_WIDGET(zone->image);
  GtkWidget *topLevel = NULL;
  topLevel = gtk_widget_get_toplevel(image);
  GtkFileFilter *filter = NULL;
  filter = gtk_file_filter_new();
  GtkWidget *dialog = NULL;
  dialog = gtk_file_chooser_dialog_new("Open file", GTK_WINDOW(topLevel),
				       GTK_FILE_CHOOSER_ACTION_OPEN,
				       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				       NULL);

  gtk_file_filter_add_pixbuf_formats(filter);
  gtk_file_filter_set_name(filter, "Image File");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

  switch(gtk_dialog_run(GTK_DIALOG(dialog)))
  {
    case GTK_RESPONSE_ACCEPT:
    {
      gchar *fileName = NULL;
      fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      gtk_image_set_from_file(GTK_IMAGE(image), fileName);
      g_free(fileName);
      break;
    }
    default:
      break;
  }
  gtk_widget_destroy(dialog);
  (void)widget;
}

void cb_save(GtkWidget *widget, gpointer data)
{
  Zone *zone = (Zone*)data;
  ///GtkWidget *text = NULL;
  ///text = GTK_WIDGET(zone->text);
  ///GtkWidget *topLevel = NULL;
  ///topLevel = gtk_widget_get_toplevel(text);
  GtkWidget *dialog = NULL;
  dialog = gtk_file_chooser_dialog_new("Save file", NULL,
				       GTK_FILE_CHOOSER_ACTION_SAVE,
				       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				       GTK_STOCK_OK, GTK_RESPONSE_ACCEPT,
				       NULL);

  GtkFileFilter *filter = NULL;
  filter = gtk_file_filter_new();
  gtk_file_filter_set_name(filter, "Text File (*.txt)");
  gtk_file_filter_add_pattern(filter, "*txt");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
  GtkFileFilter *all = NULL;
  all = gtk_file_filter_new();
  gtk_file_filter_set_name(all, "All Files");
  gtk_file_filter_add_pattern(all, "*");
  gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), all);
  gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog),
						 TRUE);
  gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog), "New Document");

  switch(gtk_dialog_run(GTK_DIALOG(dialog)))
  {
    case GTK_RESPONSE_ACCEPT:
    {
      gchar *fileName = NULL;
      fileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
      FILE *file = NULL;
      file = fopen(fileName, "w");
      if(file)
      {
	gchar *chars = NULL;
	GtkTextBuffer *textBuffer = NULL;
	textBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(zone->text));
	GtkTextIter start;
	GtkTextIter end;
	gtk_text_buffer_get_bounds(textBuffer, &start, &end);
	chars = gtk_text_buffer_get_text(textBuffer, &start, &end, FALSE);
	fprintf (file, "%s", chars);
	fclose(file);
	g_free(chars);
      }
      else
	printf("Impossible to save file\n");
      break;
    }
    default:
      break;
  }
  gtk_widget_destroy(dialog);
  (void)widget;
}

GtkWidget *guiInit(void)
{
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
  return mainBox;
}

void initMenu(GtkWidget *box, Zone *zone)
{
  /*Menu Bar*/
  GtkWidget *menuBar = NULL;
  menuBar = gtk_menu_bar_new();

  /*Menu 1*/
  GtkWidget *menu = NULL;
  menu = gtk_menu_new();
  /*Menu items*/
  GtkWidget *menuItem = NULL;
  menuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_OPEN, NULL);
  g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(cb_open), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_EXECUTE, NULL);
  g_signal_connect(G_OBJECT(menuItem), "activate",
		   G_CALLBACK(cb_process), zone);
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuItem);

  menuItem = gtk_image_menu_item_new_from_stock(GTK_STOCK_SAVE, NULL);
  g_signal_connect(G_OBJECT(menuItem), "activate", G_CALLBACK(cb_save), zone);
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
  /*Submenu*/
  menuItem = gtk_menu_item_new_with_label("?");
  /*Link menu items*/
  gtk_menu_item_set_submenu(GTK_MENU_ITEM(menuItem), menu);
  /*Add to menuBar*/
  gtk_menu_shell_append(GTK_MENU_SHELL(menuBar), menuItem);

  /*Add bar to window*/
  gtk_box_pack_start(GTK_BOX(box), menuBar, FALSE, FALSE, 0);
}

void initToolBar(GtkWidget *box, Zone *zone)
{
  GtkWidget *toolBar = gtk_toolbar_new();
  gtk_box_pack_start(GTK_BOX(box), toolBar, FALSE, FALSE, 0);

  gtk_toolbar_insert_stock(GTK_TOOLBAR(toolBar), GTK_STOCK_OPEN, "Open",
			   NULL, G_CALLBACK(cb_open), zone, -1);
  gtk_toolbar_insert_stock(GTK_TOOLBAR(toolBar), GTK_STOCK_EXECUTE, "Process",
			   NULL, G_CALLBACK(cb_process), zone, -1);
  gtk_toolbar_insert_stock(GTK_TOOLBAR(toolBar), GTK_STOCK_SAVE, "Save",
			   NULL, G_CALLBACK(cb_save), zone, -1);
  gtk_toolbar_insert_stock(GTK_TOOLBAR(toolBar), GTK_STOCK_QUIT, "Quit",
			   NULL, G_CALLBACK(cb_quit), NULL, -1);

  gtk_toolbar_set_style(GTK_TOOLBAR(toolBar), GTK_TOOLBAR_ICONS);
  gtk_toolbar_set_icon_size(GTK_TOOLBAR(toolBar), GTK_ICON_SIZE_SMALL_TOOLBAR);
}

GtkWidget *getMainZone(GtkWidget *box)
{
  GtkWidget *zone = NULL;
  zone = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_end(GTK_BOX(box), zone, TRUE, TRUE, 0);
  return zone;
}
