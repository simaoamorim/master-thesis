CC = gcc
CFLAGS = -Wall
LDFLAGS =

all: pid.o

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $^ -o $@

.PHONY: clean

clean:
	-rm *.o