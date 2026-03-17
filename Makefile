# COP4610 - Programming Assignment 2: Multithreaded Sorting
# Build on FIU Ocelot server (Linux/UNIX)

CC = gcc
CFLAGS = -Wall -pthread

all: sort

sort: sort.c
	$(CC) $(CFLAGS) -o sort sort.c

clean:
	rm -f sort
