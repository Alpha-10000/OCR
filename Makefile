#Reserved names
.PHONY: clean, mrproper

#Variables
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99
LDFLAGS=-lSDL -lSDLmain -lSDL_image
EXEC=Program
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

#Main
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $^ -o $(EXEC) $(CFLAGS) $(LDFLAGS) 

#Headers

main.o : filters.h

#Object files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) 

#Clean
clean:
	rm -rf *.o

#Mrproper
mrproper: clean
	rm -rf $(EXEC)