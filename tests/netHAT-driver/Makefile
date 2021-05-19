CC = arm-linux-gnueabihf-clang
CFLAGS ?= -Wall -g
LDFLAGS ?= -lcifx

all: driver

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

driver: driver.o
	$(CC) $^ -o $@ $(LDFLAGS)

.PHONY: all clean

clean:
	@find . -type f \( -iname '*.o' -or -iname 'driver' \) -exec rm -v {} \;
 