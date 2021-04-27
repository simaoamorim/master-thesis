CC = gcc
CFLAGS = -Wall
LDFLAGS = -lgpiod

all: quad_enc.o

quad_enc.o: quad_enc.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o ${PWD}/$@

test: quad_enc_test.c quad_enc.o
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o ${PWD}/$@

.PHONY: clean

clean:
	find ${PWD} \( -iname '*.o' -or -iname 'test' \) -exec rm {} \;
	#find ${PWD} -iname 'test' -exec rm {} \;
