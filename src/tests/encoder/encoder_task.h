#ifndef _ENCODER_TASK_H_
#define _ENCODER_TASK_H

#include "encoder.h"
#include <signal.h>
#include <unistd.h>
#include <sched.h>
#include <stdio.h>

struct encoder_task {
	struct encoder encoder;
	pthread_mutex_t counter_mutex;
	int period;
};

void * encoder_task (void *args);

long encoder_task_get_count (struct encoder_task *e);

void encoder_task_stop ();

#endif