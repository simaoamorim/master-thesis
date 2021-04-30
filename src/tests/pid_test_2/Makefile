CC = gcc
CFLAGS = -Wall -I../encoder/ -I../pid/ -I../DFR0592/
LDFLAGS = -li2c -lgpiod

VPATH = ${PWD}

all: pid_test_2

pid_test_2.o: pid_test_2.c
	$(CC) $(CFLAGS) -c $< -o $@

pid_test_2: pid_test_2.o encoder.o pid.o dfr0592.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

encoder.o:
	@$(MAKE) VPATH=../encoder/ -f ../encoder/Makefile $@

pid.o:
	@$(MAKE) VPATH=../pid/ -f ../pid/Makefile $@

dfr0592.o:
	@$(MAKE) VPATH=../DFR0592/ -f ../DFR0592/Makefile $@

.PHONY: clean all

clean:
	@find ${PWD} -type f -iname 'pid_test_2' -exec rm {} \;
	@find ${OBJPATH} -type f -iname '*.o' -exec rm {} \;
	
