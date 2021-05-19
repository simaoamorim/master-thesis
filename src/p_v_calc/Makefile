CC ?= gcc
CFLAGS ?= -Wall -I../encoder/
LDFLAGS ?=

all: p_v_calc.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	@find . -type f -iname '*.o' -exec rm -v {} \;
