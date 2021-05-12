#ifndef CONTROL_H_
#define CONTROL_H_

#define	NEW_CONTROL_S {\
	.pid_pos = NULL,\
	.pid_vel = NULL,\
	.period = 1000,\
	.priority = 11,\
	.enabled = false,\
	.keep_running = true,\
}

#include <pthread.h>
#include <sched.h>
#include <stdbool.h>
#include <unistd.h>
#include <pid.h>

struct control_s {
	struct pid_s *pid_pos;
	struct pid_s *pid_vel;
	long period;
	long priority;
	bool enabled;
	bool keep_running;
};

void * control_task (void *arg);

void control_task_stop (struct control_s *cs);

#endif
