CC = gcc
CFLAGS = -Wall -lrt

all: timersig

timersig: timersig.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean

clean:
	-rm -v timersig
