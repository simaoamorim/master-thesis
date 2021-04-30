CC = gcc
CFLAGS = -Wall
LDFLAGS = -lgpiod

all: encoder.o encoder_task.o

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o $@

test: encoder_test.c encoder.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean

clean:
	find ${PWD} \( -iname '*.o' -or -iname 'test' \) -exec rm {} \;
