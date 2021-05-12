CC = clang
CFLAGS = -Wall -I../encoder/ -I../pid/ -I../DFR0592/ -I../p_v_calc/ -I../control/
LDFLAGS = -li2c -lgpiod -lpthread
ENCOBJ = encoder.o encoder_task.o

VPATH = ${PWD}

all: main

main.o: main.c
	$(CC) $(CFLAGS) -c $< -o $@

main: main.o encoder.o encoder_task.o pid.o dfr0592.o p_v_calc.o control.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(ENCOBJ):
	@$(MAKE) VPATH=../encoder/ -f ../encoder/Makefile $@

pid.o:
	@$(MAKE) VPATH=../pid/ -f ../pid/Makefile $@

dfr0592.o:
	@$(MAKE) VPATH=../DFR0592/ -f ../DFR0592/Makefile $@

p_v_calc.o:
	@$(MAKE) VPATH=../p_v_calc/ -f ../p_v_calc/Makefile $@

control.o:
	@$(MAKE) VPATH=../control/ -f ../control/Makefile $@

.PHONY: clean all $(ENCOBJ) pid.o dfr0592.o p_v_calc.o

clean:
	@find ${PWD} -type f \( -iname 'main' -or -iname '*.o' \) -exec rm -v {} \;
	
