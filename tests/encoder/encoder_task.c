#include "encoder_task.h"

int encoder_task_keep_running = 1;

void * encoder_task (void *args)
{
	struct sched_param sched_params = {.sched_priority = 11};
	if (0 != sched_setscheduler(0, SCHED_FIFO, &sched_params)) {
		pthread_exit((void *) -1);
	}
	struct encoder_task *e = (struct encoder_task *) args;
	pthread_mutex_init(&e->counter_mutex, NULL);
	while (encoder_task_keep_running) {
		encoder_read_values(&e->encoder);
		encoder_decode_stage(&e->encoder);
		pthread_mutex_lock(&e->counter_mutex);
		encoder_update_counter(&e->encoder);
		pthread_mutex_unlock(&e->counter_mutex);
		usleep(10);
	}
	pthread_mutex_destroy(&e->counter_mutex);
	pthread_exit((void *) 0);
}

long encoder_task_get_count (struct encoder_task *e)
{
	static long tmp;
	pthread_mutex_lock(&e->counter_mutex);
	tmp = e->encoder.count;
	pthread_mutex_unlock(&e->counter_mutex);
	return tmp;
}

void encoder_task_stop ()
{
	encoder_task_keep_running = 0;
}
