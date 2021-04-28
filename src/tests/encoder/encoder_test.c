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
	printf("Number of bulked lines: %d\n", gpiod_line_bulk_num_lines(enc.inputs));
	puts("Press enter to continue...");
	getchar();

	while (keep_running) {
		printf("\e[1;1H\e[2J"); // clear console
		encoder_read_values(&enc);
		encoder_decode_stage(&enc);
		encoder_update_counter(&enc);
		printf("Line A: %d\n", enc.values[0]);
		printf("Line B: %d\n", enc.values[1]);
		printf("Stage: %d\n", enc.stage);
		printf("New stage: %d\n", enc.new_stage);
		printf("Count: %ld\n", enc.count);
		fflush(stdout);
		usleep(10);
	}

end:	
	printf("\33[2K\n");
	encoder_end(&enc);
	return ret;
}
