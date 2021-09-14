#include "control.h"

#define	delta(t1, t2) (t2.tv_sec - t1.tv_sec) + ((t2.tv_nsec - t1.tv_nsec) / 1000000000.0)

void * control_task (void *arg)
{
	struct control_s *cs = (struct control_s *) arg;
	struct timespec prev_time, cur_time;
	int lret;
	int comm_ok = 0, enable = 0, comm_nok_prev = 0, first_run = 1;

	lret = comm_bus_config_lock(cs->comm_s);
	if (0 != lret)
		goto error_exit;

	do {
		clock_gettime(CLOCK_MONOTONIC, &cur_time);

		comm_ok = comm_bus_active(cs->comm_s);

		if (comm_ok) {
			comm_update_inputs(cs->comm_s);
			if (comm_nok_prev) {
				puts("Communication established!");
				comm_nok_prev = 0;
			}
			enable = comm_get_input_bit(cs->comm_s, 6, 0);

			if (enable) {
				// Control loop here
				p_v_enable_task(cs->pv_s);
				// Get inputs
				cs->pid_vel->command = comm_get_input_word(cs->comm_s, 2);
				cs->pid_vel->max_output_delta = comm_get_input_word(cs->comm_s, 4);
				if (cs->pid_vel->feedback_var == VELOCITY)
					cs->pid_vel->feedback = p_v_get_velocity(cs->pv_s);
				else if (cs->pid_vel->feedback_var == POSITION)
					cs->pid_vel->feedback = p_v_get_position(cs->pv_s);
				cs->pid_vel->delta_t = delta(prev_time, cur_time);
				// Update outputs
				comm_put_output_word(cs->comm_s, 0, (uint16_t) p_v_get_velocity(cs->pv_s));
				comm_put_output_word(cs->comm_s, 2, (uint16_t) p_v_get_position(cs->pv_s));
				comm_put_output_word(cs->comm_s, 4, (uint16_t) encoder_task_get_count(cs->enc_task));
				// Do calculations and set motor speed
				if (cs->remote_mode) {
					motor_set_speed(cs->dfr_board, 1, (float) comm_get_input_word(cs->comm_s, 0));
				} else {
					do_calcs(cs->pid_vel);
					motor_set_speed(cs->dfr_board, 1, (float) get_output(cs->pid_vel));
				}
				comm_update_outputs(cs->comm_s);
			} else {
				motor_stop(cs->dfr_board, 1);
				p_v_disable_task(cs->pv_s);
			}
		} else {
			if (!comm_nok_prev) {
				if (!first_run)
					puts("Communication lost!");
				puts("Waiting to establish communication with master ...");
				motor_stop(cs->dfr_board, 1);
				p_v_disable_task(cs->pv_s);
				comm_nok_prev = 1;
			}
		}

		prev_time = cur_time;

		if (first_run)
			first_run = 0;

		usleep(cs->period);

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
