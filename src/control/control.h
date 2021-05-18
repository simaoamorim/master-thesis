#ifndef CONTROL_H_
#define CONTROL_H_

#define	NEW_CONTROL_S {\
	.pid_vel = NULL,\
	.pv_s = NULL,\
	.dfr_board = NULL,\
	.comm_s = NULL,\
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
#include <dfr0592.h>
#include <p_v_calc.h>
#include <comm.h>

struct control_s {
	struct pid_s *pid_vel;
	struct p_v_task_s *pv_s;
	struct dfr_board *dfr_board;
	struct comm_s *comm_s;
	long period;
	long priority;
	bool enabled;
	bool keep_running;
};

void * control_task (void *arg);

void control_task_stop (struct control_s *cs);

void control_task_set_velocity_command (struct control_s *cs, double command);

#endif
