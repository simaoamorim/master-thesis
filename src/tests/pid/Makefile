CC = gcc
CFLAGS = -Wall
LDFLAGS = -I.
TARGET = pid.o

all: ${TARGET}

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test: pid_test.c ${TARGET}
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean

clean:
	find . -name '*.o' -or -name 'test' -exec rm {} +