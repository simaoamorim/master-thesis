CC = gcc
CFLAGS = -Wall
LDFLAGS = -I.
TARGET ?= dfr0592_test
OBJECTS ?= dfr0592.o

all: $(TARGET)

$(TARGET) : $(TARGET).c $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c -fpic $< -o $@

%.so : %.o
	$(CC) $(CFLAGS) -shared $< -o $@

.PHONY: clean

clean:
	@if [ -f "$(TARGET)" ]; then rm $(TARGET); fi
	@if [ -f "$(OBJECTS)" ]; then rm $(OBJECTS); fi
