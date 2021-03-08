CFLAGS = -Wall
LDFLAGS = -lcifx
#LDFLAGS += -lpthread -lrt

all: demo

demo: demo.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean

clean:
	-rm -v demo
