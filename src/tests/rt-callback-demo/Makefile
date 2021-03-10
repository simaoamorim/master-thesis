CC = gcc
CFLAGS = -Wall
LDFLAGS =
TARGET = rt-callback-demo

all: rt-callback-demo

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

.PHONY: clean

clean:
	-rm -v $(TARGET)
