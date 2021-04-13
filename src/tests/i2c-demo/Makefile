CC = gcc
CFLAGS = -Wall
LDFLAGS = -li2c
TARGET = i2c-demo
OBJECTS =

all: $(TARGET)

$(TARGET): $(TARGET).c $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	-rm -v $(TARGET)
	-rm -v $(OBJECTS)
