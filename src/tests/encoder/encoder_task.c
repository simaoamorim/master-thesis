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
	struct sched_param sched_params = {.sched_priority = 20,};
	if (-1 == sched_setscheduler(0, SCHED_FIFO, &sched_params)) {
		pthread_exit((void *) -1);
	}
	struct encoder *enc;
	enc = (struct encoder *) args;
	while (keep_running) {
		encoder_read_values(enc);
		encoder_decode_stage(enc);
		encoder_update_counter(enc);
		usleep(150);
	}
	pthread_exit((void *) 0);
}
