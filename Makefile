#Reserved names
.PHONY: clean, mrproper

#Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99

#Main
all: main.o   #add required .o files
	$(CC) $^ -o Program $(CFLAGS)

#Headers


#Object files
%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

#Clean
clean:
	rm -rf *.o

#Mrproper
mrproper: clean
	rm -rf Program