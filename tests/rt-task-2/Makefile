CC = gcc
CFLAGS = -Wall -lrt
TARGET = rt-task

all: rt-task

$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) $< -o $@

.PHONY: all clean setuid help

setuid: $(TARGET)
	chown root:root $<
	chmod u+s $<

clean:
	-rm -v $(TARGET)

help:
	@echo "Usage:"
	@printf "\tmake [target]\n"
	@echo ""
	@echo "Targets:"
	@printf "\thelp\tprint this help message\n"
	@printf "\t$(TARGET)\tbuild the executable\n"
	@printf "\tsetuid\tbuild the executable, make root its owner and set the UID bit\n"
	@printf "\tall\t$(TARGET)\n"
	@echo ""
	@printf "If no target is specified, the default is <all>\n"
