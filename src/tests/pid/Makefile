CC ?= gcc
CFLAGS ?= -Wall -g
LDFLAGS ?= -I.
TARGET ?= pid.o
ODIR ?= ${PWD}

all: ${TARGET}

%.o: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) -c $< -o ${ODIR}/$@

test: pid_test.c ${TARGET}
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

.PHONY: clean

clean:
	find ${ODIR} -iname '*.o' -exec rm {} \;
	find . -iname 'test' -exec rm {} \;
