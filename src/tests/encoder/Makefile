CC = gcc
CFLAGS = -Wall
LDFLAGS = -lgpiod

all: encoder.o

encoder.o: encoder.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o ${PWD}/$@

test: encoder_test.c encoder.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o ${PWD}/$@

.PHONY: clean

clean:
	find ${PWD} \( -iname '*.o' -or -iname 'test' \) -exec rm {} \;
