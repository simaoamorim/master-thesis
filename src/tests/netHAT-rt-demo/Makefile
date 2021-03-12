CC = gcc
CFLAGS = -Wall
LDFLAGS = -lcifx
#LDFLAGS += -lrt
#LDFLAGS += -lpthread
TARGET = demo
OBJECTS = sched.o

all: $(TARGET)

$(TARGET): $(TARGET).c $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	-rm $(TARGET) $(OBJECTS)
