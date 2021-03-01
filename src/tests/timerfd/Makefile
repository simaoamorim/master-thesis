CC = gcc
CFLAGS = -Wall

all: timerfd

timerfd: timerfd.c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean all

clean:
	rm -v timerfd
