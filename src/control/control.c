#include "control.h"

#define	delta(t1, t2) (t2.tv_sec - t1.tv_sec) + ((t2.tv_nsec - t1.tv_nsec) / 1000000000.0)

void * control_task (void *arg)
{
	struct control_s *cs = (struct control_s *) arg;
	struct timespec prev_time, cur_time;
	int lret;

	lret = comm_bus_config_lock(cs->comm_s);
	if (0 != lret)
		goto error_exit;

	clock_gettime(CLOCK_MONOTONIC, &prev_time);

	do {
		if (comm_bus_active(cs->comm_s)) {
			// Control loop here
			usleep(cs->period);
			clock_gettime(CLOCK_MONOTONIC, &cur_time);
			comm_update_inputs(cs->comm_s);

			// Get inputs
			cs->pid_vel->command = comm_get_input_word(cs->comm_s, 0);
			printf("Input value: %4.3f\n", cs->pid_vel->command);
			cs->pid_vel->feedback = p_v_get_velocity(cs->pv_s);
			cs->pid_vel->delta_t = delta(prev_time, cur_time);

			// Do calculations
			do_calcs(cs->pid_vel);

			// Update outputs
			motor_set_speed(cs->dfr_board, 1, (float) get_output(cs->pid_vel) );

			prev_time = cur_time;
		} else {
			puts("Waiting to establish communication with master...");
		}
	} while (cs->keep_running);

	pthread_exit((void *) 0);
error_exit:
	pthread_exit((void *) -1);
}

void control_task_stop (struct control_s *cs)
{
	cs->keep_running = false;
}

void control_task_set_velocity_command (struct control_s *cs, double command)
{
	cs->pid_vel->command = command;
}
