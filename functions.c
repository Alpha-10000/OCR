#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <gtk/gtk.h>
#include "functions.h"
#include "filters.h"
#include "network.h"
#include "detection.h"
#include "gui.h"
#include "rotation.h"
#include "save.h"
#include "angle.h"

SDL_Surface *copySurface(SDL_Surface *surface)
{
    SDL_Surface *copy = NULL;
    copy = SDL_CreateRGBSurface(
            SDL_HWSURFACE, surface->w, surface->h, 32,
            surface->format->Rmask, surface->format->Gmask,
            surface->format->Bmask, surface->format->Amask);
    if(surface == NULL || copy == NULL)
        return NULL;
    SDL_FreeSurface(copy);
    copy = NULL;
    return SDL_DisplayFormatAlpha(surface);
}
void sortArray(Uint8 array[], int size)
{
    for(int i = 0; i < size; i++)
    {
        Uint8 temp = array[i];
        int j = i;
        while(j >= 0 && array[j-1] > temp)
        {
            array[j] = array[j-1];
            j--;
        }
        array[j] = temp;
    }
}

void printAllocError(void)
{
    printf("Error : alloc fail");
    exit(EXIT_FAILURE);
}

void cb_process(GtkWidget *widget, gpointer data)
{
    Zone *zone = (Zone*)data;
    GtkWidget * image = NULL;
    image = GTK_WIDGET(zone->image);
    GdkPixbuf *pixBuf = NULL;
    pixBuf = gtk_image_get_pixbuf(GTK_IMAGE(image));
    if(pixBuf)
    {
        gdk_pixbuf_save(pixBuf, "data.bmp", "bmp", NULL, NULL, NULL);
        SDL_Surface *textImage = NULL;
        textImage = IMG_Load("data.bmp");
        if(textImage == NULL)
        {
            fprintf(stderr, "Error while loading SDL Surface\n");
            exit(EXIT_FAILURE);
        }

        /*double angle = houghHist(textImage);
        printf("Angle = %f\n", angle);
        SDL_Surface *swp = textImage;
        textImage = rotate(textImage, -angle);
        SDL_FreeSurface(swp);*/

        greyScale(textImage);
        //noiseRemove(textImage);
        binarize(textImage);

        pixBuf = loadPixBuf(textImage);
        gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixBuf);

        int nbLines;
        Block *blocks = findBlocks(textImage, &nbLines);
        //print_blocks(blocks, nbLines);
        findChars(textImage, blocks, nbLines);

        //printMedianCharSize(blocks, nbLines);
        //printSpaces(blocks, nbLines);
        //drawLinesChars(textImage, blocks, nbLines);

        textImage = resizeChars(textImage, blocks, nbLines);

        network *testNN = initNetwork(3,40);
        readNetworkSettings(testNN);
        //learnNetwork(testNN, blocks, textImage, nbLines);

        //pixBuf = loadPixBuf(textImage);
        //gtk_image_set_from_pixbuf(GTK_IMAGE(image), pixBuf);
        /*
           int *entryVector = malloc(NN_RESOLUTION*NN_RESOLUTION*sizeof(int));
           fillEntryVector(textImage, entryVector,
           getCharNb(15, blocks, nbLines),
           getLineNb(15, blocks, nbLines));
           computeOutput(testNN, entryVector);
           printOutput(testNN);
           free(entryVector);


           int line = 6;
           for (int i = 0; i < blocks[line].nbChars; i++)
           {
           printf("x=%d  ", blocks[line].chars[i].x);
           printf("y=%d  ", blocks[line].chars[i].y);
           printf("w=%d  ", blocks[line].chars[i].w);
           printf("h=%d\n", blocks[line].chars[i].h);
           }*/
        //saveNetworkSettings(testNN);
        //readNetworkSettings(testNN);

        char* chars = NULL;
        chars = malloc(nbLines * 200 * sizeof(char));
        for(int i = 0; i < 200 * nbLines; i++)
            chars[i] = '\0';
	chars = readText(testNN, textImage, blocks, nbLines, chars);
	displayOutput(chars, zone);

	freeNetwork(testNN);
	freeBlocks(blocks, nbLines);
	SDL_FreeSurface(textImage);
	free(chars);
    }
    (void)widget;
}
