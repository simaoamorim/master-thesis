CC = gcc
CFLAGS = -Wall
LDFLAGS = -lrt
TARGET = rt-callback-demo

all: rt-callback-demo

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean

clean:
	-rm -v $(TARGET)
