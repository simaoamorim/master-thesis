CC ?= gcc
CFLAGS ?= -Wall -g
LDFLAGS ?= -I. -li2c
TARGET ?= dfr0592_test
OBJECTS ?= dfr0592.o
DOCS ?= docs/
ODIR ?= ${PWD}

all: $(OBJECTS)

$(TARGET) : $(TARGET).c $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c -fpic $< -o ${ODIR}/$@

%.so : %.o
	$(CC) $(CFLAGS) -shared $< -o $@

docs: Doxyfile
	doxygen Doxyfile

docs_edit: Doxyfile
	doxywizard Doxyfile

test: dfr0592_test

.PHONY: clean

clean:
	@if [ -f "$(TARGET)" ]; then rm -v $(TARGET); fi
	@if [ -f "$(OBJECTS)" ]; then rm -v $(OBJECTS); fi
	@if [ -d "$(DOCS)" ]; then rm -rv $(DOCS); fi
