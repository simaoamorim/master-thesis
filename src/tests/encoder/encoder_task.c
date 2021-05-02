#include "encoder_task.h"

void * encoder_task (void *args)
{
	struct sched_param sched_params = {.sched_priority = 1};
	if (0 != sched_setscheduler(0, SCHED_FIFO, &sched_params)) {
		pthread_exit((void *) -1);
	}
	struct encoder_task *e = (struct encoder_task *) args;
	pthread_mutex_init(&e->counter_mutex, NULL);
	while (1) {
		encoder_read_values(&e->encoder);
		encoder_decode_stage(&e->encoder);
		pthread_mutex_lock(&e->counter_mutex);
		encoder_update_counter(&e->encoder);
		pthread_mutex_unlock(&e->counter_mutex);
		usleep(150);
	}
	printf("Exited encoder loop\n");
	pthread_exit((void *) 0);
}

void encoder_task_cleanup (struct encoder_task *e)
{
	pthread_mutex_destroy(&e->counter_mutex);
}

long encoder_task_get_count (struct encoder_task *e)
{
	long tmp;
	pthread_mutex_lock(&e->counter_mutex);
	tmp = e->encoder.count;
	pthread_mutex_unlock(&e->counter_mutex);
	return tmp;
}
