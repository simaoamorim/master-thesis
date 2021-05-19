CC ?= clang
CFLAGS ?= -Wall
LDFLAGS ?=

all: comm.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	@find . -type f -iname '*.o' -exec rm -v {} \;
