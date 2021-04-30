#ifndef _ENCODER_TASK_H_
#define _ENCODER_TASK_H

#include "encoder.h"
#include <signal.h>
#include <unistd.h>
#include <sched.h>

void * encoder_task (void *args);

long encoder_task_get_count (struct encoder *e);

#endif