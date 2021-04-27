#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#include <gpiod.h>

#include "encoder.h"

volatile int keep_running = 1;

void sighandler (int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
	}
}

int main (int argc, char *argv[])
{
	int ret = 0;
	signal(SIGINT, sighandler);
	struct encoder enc = {0};
	if (-1 == encoder_init(&enc, 0, 17, 18)) {
		perror("Failed to initialize 'encoder'");
		ret = -1;
		goto end;
	}
	puts("Encoder open OK");
	if (-1 == encoder_start(&enc)) {
		ret = -1;
		perror("Failed to request events for the inputs");
		goto end;
	}
	int vals[2];
	while (keep_running) {
		if (1 == encoder_wait(&enc)) {
			printf("\e[1;1H\e[2J"); // clear console
			gpiod_line_get_value_bulk(enc.inputs, vals);
			printf("Line A: %d\n", vals[0]);
			printf("Line B: %d\n", vals[1]);
			int size = gpiod_line_bulk_num_lines(enc.events);
			for (int i = 1; i <= size; i++) {
				struct gpiod_line_event event;
				gpiod_line_event_read(
					gpiod_line_bulk_get_line(enc.events, i),
					&event);
				printf("Event on line %d: %d\n",
					gpiod_line_offset(gpiod_line_bulk_get_line(enc.events,i)),
					event.event_type);
			}
		}
	}

end:	
	printf("\33[2K\n");
	encoder_end(&enc);
	return ret;
}
