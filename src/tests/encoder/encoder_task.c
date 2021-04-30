#include "encoder_task.h"

int keep_running = 1;

void sighandler (int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
	}
}

void * encoder_task (void *args)
{
	if (SIG_ERR == signal(SIGINT, sighandler)) {
		pthread_exit((void *) -1);
	}
	puts("Thread: signal OK");
	struct encoder *enc;
	enc = (struct encoder *) args;
	puts("Thread: entering loop");
	while (keep_running) {
		puts("Thread: read inputs");
		encoder_read_values(enc);
		puts("Thread: decode stage");
		encoder_decode_stage(enc);
		puts("Thread: update counter");
		encoder_update_counter(enc);
	}
	puts("Thread: terminating");
	pthread_exit((void *) 0);
}
