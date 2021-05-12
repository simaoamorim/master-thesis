#include "control.h"

#define	delta(t1, t2) (t2.tv_sec - t1.tv_sec) + ((t2.tv_nsec - t1.tv_nsec) / 1000000000.0)

void * control_task (void *arg)
{
	struct control_s *cs = (struct control_s *) arg;
	struct timespec prev_time, cur_time;

	if (cs->pid_pos != NULL)
		puts("Using position PID");
	if (cs->pid_vel != NULL)
		puts("Using velocity PID");

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
