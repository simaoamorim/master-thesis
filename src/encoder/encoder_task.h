#ifndef ENCODER_TASK_H_
#define ENCODER_TASK_H_

#include "encoder.h"
#include <signal.h>
#include <unistd.h>
#include <sched.h>

struct encoder_task {
	struct encoder encoder;
	pthread_mutex_t counter_mutex;
	int period;
};

extern int encoder_task_keep_running;

void * encoder_task (void *args);

long encoder_task_get_count (struct encoder_task *e);

void encoder_task_stop ();

#endif