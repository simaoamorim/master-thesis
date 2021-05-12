#include "control.h"

void * control_task (void *arg)
{
	struct control_s *cs = (struct control_s *) arg;

	struct timespec prev_time, cur_time;

	clock_gettime(CLOCK_MONOTONIC, &prev_time);

	do {
		// Control loop here
		usleep(cs->period);
		clock_gettime(CLOCK_MONOTONIC, &cur_time);

		prev_time = cur_time;
	} while (cs->keep_running);

	pthread_exit((void *) 0);
fail:
	pthread_exit((void *) -1);
}

void control_task_stop (struct control_s *cs)
{
	cs->keep_running = false;
}
