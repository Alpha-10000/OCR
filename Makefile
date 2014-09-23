#Reserved names
.PHONY: clean, mrproper

#Variables
CC=gcc
CFLAGS=-Wall -Wextra -Werror -std=c99
LDFLAGS=-lSDL -lSDLmain -lSDL_image
EXEC=Program

#Main
all: $(EXEC)

$(EXEC): main.o   #add required .o files
	$(CC) $^ -o $(EXEC) $(CFLAGS) $(LDFLAGS) 

#Headers


#Object files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) 

#Clean
clean:
	rm -rf *.o

#Mrproper
mrproper: clean
	rm -rf $(EXEC)