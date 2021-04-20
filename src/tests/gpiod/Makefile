CC = gcc
CFLAGS = -Wall
LDFLAGS = -lgpiod
TARGET = main

all: ${TARGET}

${TARGET}: ${TARGET}.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean

clean:
	-rm ${TARGET}
