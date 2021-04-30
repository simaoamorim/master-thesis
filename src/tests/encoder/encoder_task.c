#include "encoder_task.h"

int keep_running = 1;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

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
	struct sched_param sched_params = {.sched_priority = 1};
	if (0 != sched_setscheduler(0, SCHED_FIFO, &sched_params)) {
		pthread_exit((void *) -1);
	}
	struct encoder *enc = (struct encoder *) args;
	//pthread_mutex_init(counter_mutex, NULL);
	while (keep_running) {
		encoder_read_values(enc);
		encoder_decode_stage(enc);
		pthread_mutex_lock(&counter_mutex);
		encoder_update_counter(enc);
		pthread_mutex_unlock(&counter_mutex);
		usleep(150);
	}
	pthread_mutex_destroy(&counter_mutex);
	pthread_exit((void *) 0);
}
