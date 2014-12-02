#Reserved names
.PHONY: clean, mrproper

#Variables
CC=clang
CFLAGS=-Wall -Wextra -Werror -std=c99 -O2 $(shell pkg-config --cflags gtk+-2.0 gtkspell-2.0)
LDFLAGS=-lSDL -lSDLmain -lSDL_image -lm $(shell pkg-config --libs gtk+-2.0 gtkspell-2.0)
EXEC=OCRulus
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

#Main
all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) $^ -o $(EXEC) $(CFLAGS) $(LDFLAGS)

#Object files
%.o: %.c
	@$(CC) -c $< -o $@ $(CFLAGS)

#Clean
clean:
	rm -rf *.o *~ *# *.swp data.bmp

#Mrproper
mrproper: clean
	rm -rf $(EXEC)