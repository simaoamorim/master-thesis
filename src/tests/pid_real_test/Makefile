CC = gcc
CFLAGS = -Wall -I../DFR0592/ -I../pid/
LDFLAGS = -li2c
TARGET = pid_real_test
ORBJECTS = pid.o dfr0592.o
SUBDIRS = ../pid/ ../dfr0592/
ODIR ?= ${PWD}
export ${ODIR}

all: ${TARGET}

pid_real_test: pid_real_test.c pid.o dfr0592.o 
	$(CC) $(CFLAGS) -static -o $@ $^ $(LDFLAGS)

pid.o: 
	$(MAKE) -C../pid/ $@

dfr0592.o:
	$(MAKE) -C../DFR0592/ $@

.PHONY: clean

clean:
	find ${ODIR}/ -iname '${TARGET}' -exec rm {} \;
	$(MAKE) -C../pid/ clean
	$(MAKE) -C../DFR0592/ clean