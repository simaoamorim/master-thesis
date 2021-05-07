CC = gcc
CFLAGS = -Wall
LDFLAGS =
TARGET ?= main

all: cmdline.c ${TARGET}

main.o: main.c cmdline.c
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

main: main.o cmdline.o
	$(CC) $^ -o $@ $(LDFLAGS)

cmdline.c: test.ggo
	gengetopt --conf-parser --input=test.ggo

doc:
	doxygen Doxyfile

.PHONY: all clean doc

clean:
	@find . -type f \( -iname '${TARGET}' -or -iname '*.o' \) -exec rm -v {} \;
 