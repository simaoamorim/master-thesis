CC = gcc
CFLAGS = -Wall
LDFLAGS =

all: rt-deadline-task

rt-deadline-task: rt-deadline-task.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -v rt-deadline-task
