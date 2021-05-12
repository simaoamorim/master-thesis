CC = gcc
CFLAGS = -Wall -I.
LDFLAGS = -static

all: sched_funcs.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS)

.PHONY: all clean

clean:
	@find ${PWD} -type f -iname '*.o' -exec rm {} \;