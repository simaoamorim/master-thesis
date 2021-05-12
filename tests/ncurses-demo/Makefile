CC = gcc
CFLAGS = -Wall -lncurses
OBJ = ncurses-demo

all: $(OBJ)

$(OBJ): $(OBJ).c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean all

clean:
	rm -v $(OBJ)
