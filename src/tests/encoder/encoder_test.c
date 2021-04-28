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
	if (-1 == encoder_start(&enc)) {
		perror("Failed to request events on the encoder");
		ret = -1;
		goto end;
	}
	puts("Encoder open OK");
	printf("Number of bulked lines: %d\n", gpiod_line_bulk_num_lines(enc.inputs));
	puts("Press enter to continue...");
	getchar();
	int vals[2];
	while (keep_running) {
		printf("\e[1;1H\e[2J"); // clear console
		gpiod_line_get_value_bulk(enc.inputs, vals);
		printf("Line A: %d (%d)\n", vals[0], gpiod_line_get_value(enc.a_line));
		printf("Line B: %d (%d)\n", vals[1], gpiod_line_get_value(enc.b_line));
		usleep(10);
	}

end:	
	printf("\33[2K\n");
	encoder_end(&enc);
	return ret;
}
