CC = gcc
CFLAGS = -Wall -I.
LDFLAGS = -lgpiod -pthread

all: encoder.o encoder_task.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: encoder_test.c encoder.o | test_task
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

test_task: encoder_task_test.c encoder.o encoder_task.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

.PHONY: clean

clean:
	find ${PWD} \( -iname '*.o' -or -iname 'test*' \) -exec rm -v {} \;
