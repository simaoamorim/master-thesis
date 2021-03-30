CC = gcc
CFLAGS = -Wall -lpthread
OBJ = pthreads-demo

all: $(OBJ)

$(OBJ): $(OBJ).c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean

clean:
	rm -v $(OBJ)
