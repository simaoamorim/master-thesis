CC ?= gcc
CFLAGS ?= -Wall -g
LDFLAGS ?= -I. -li2c
TARGET ?= dfr0592_test
OBJECTS ?= dfr0592.o
DOCS ?= docs/

all: $(OBJECTS)

$(TARGET) : $(TARGET).c $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

docs: Doxyfile
	doxygen Doxyfile

docs_edit: Doxyfile
	doxywizard Doxyfile

test: dfr0592_test

.PHONY: clean all
.DEFAULT: all

clean:
	@if [ -f "$(TARGET)" ]; then rm -v $(TARGET); fi
	@find ${PWD} \( -iname '*.o' -or -iname '*.so' \) -exec rm -v {} \;
	@if [ -d "$(DOCS)" ]; then rm -rv $(DOCS); fi
