CC = gcc
CFLAGS = -Wall
LDFLAGS = -lwiringPi

all: main

main: main.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.phony: clean

clean:
	-rm main
