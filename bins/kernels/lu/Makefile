#
# Copyright(C) 2014 Pedro H. Penna <pedrohenriquepenna@gmail.com>
#
INCLUDE = -I.
CC = arm-linux-gnueabihf-gcc
CFLAGS = -static -O2

# Source files.
SRC = $(wildcard *.c)

# Executable file.
EXEC = lu

# Builds kernel.
all: $(SRC)
	$(CC) $(SRC) $(CFLAGS) $(INCLUDE) -o $(EXEC).armel -lm
	
# Cleans compilation files.
clean:
	rm -f $(EXEC).armel
