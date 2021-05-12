CC = clang
CFLAGS = -Wall -I../pid/

all: control.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	@find . -type f -iname '*.o' -exec rm -v {} \;
