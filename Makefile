#Reserved names
.PHONY: clean, mrproper

#Variables
CC=clang
CFLAGS=-Wall -Wextra -Werror -std=c99 -O2 $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS=-lSDL -lSDLmain -lSDL_image -lm $(shell pkg-config --libs gtk+-2.0)
EXEC=Program
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

#Main
all: $(EXEC)

$(EXEC): $(OBJ)
	@$(CC) $^ -o $(EXEC) $(CFLAGS) $(LDFLAGS)

#Headers

main.o : filters.h functions.h neuron.h layer.h network.h detection.h

#Object files
%.o: %.c
	@$(CC) -c $< -o $@ $(CFLAGS)

#Clean
clean:
	rm -rf *.o *~ *# *.swp

#Mrproper
mrproper: clean
	rm -rf $(EXEC)
