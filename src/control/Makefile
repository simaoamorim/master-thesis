CC = clang
CFLAGS = -Wall -I../pid/ -I../DFR0592/ -I../p_v_calc/ -I../encoder/ -I../comm/

all: control.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean

clean:
	@find . -type f -iname '*.o' -exec rm -v {} \;
