#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <pid.h>
#include <dfr0592.h>

#include <stdint.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <linux/sched/types.h>
#include <signal.h>
#include <time.h>
#include "local_sched.h"

int keep_running = 1;

void sighandler(int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
		write(1, "\n", 1);
	}
	return;
}

int main (int argc, char *argv[])
{
	int lret;
	struct sched_attr attr = {
		.size = sizeof(struct sched_attr),
		.sched_policy = SCHED_DEADLINE,
		.sched_priority = 0,
		.sched_flags = 0,
		.sched_period = 1000000,
		.sched_runtime = 1000000,
		.sched_deadline = 1000000
	};
	
	if (SIG_ERR == signal(SIGINT, sighandler))
		err(EXIT_FAILURE, "signal()");
	
	lret = sched_setattr(0, &attr, 0);
	if (-1 == lret) {
		err(EXIT_FAILURE, "Failed to set SCHED_DEADLINE");
	}
	
	const struct dfr_board *board = board_init(1, 0x10);
	if (NULL == board)
		err(EXIT_FAILURE, "Failed to open board connection");
	board_set_mode(board, DC);
	set_pwm_frequency(board, 10000);
	encoder_set_ratio(board, 1, 11);
	encoder_enable(board, 1);
	
	struct pid_t pid = {
		.command = 100.0,
		.feedback = 0.0,
		.delta_t = 0.0,
		
		.p_gain = 1.0,
		.i_gain = 0.1,
		.d_gain = 0.0,
		
		.max_error = 100.0,
		.max_i_error = 100.0,
		.max_d_error = 100.0,
		.max_output = 100.0,
		
		.error = 0.0,
		.previous_error = 0.0,
	};
	
	struct timespec prev_time = {0}, cur_time = {0};
	int speed_fb;
	
	clock_gettime(CLOCK_MONOTONIC, &prev_time);
	sched_yield();
	
	while (keep_running) {
		encoder_get_speed(board, 1, &speed_fb);
		clock_gettime(CLOCK_MONOTONIC, &cur_time);
		pid.feedback = speed_fb;
		pid.delta_t = (cur_time.tv_sec - prev_time.tv_sec) * 1000000000UL + \
			(cur_time.tv_nsec - prev_time.tv_nsec);
		do_calcs(&pid);
		motor_set_speed(board, 1, (int) get_output(&pid));
		prev_time = cur_time;
		sched_yield();
	}
	
	motor_stop(board, 1);
	board_close(board);
	free((struct dfr_board*) board);
	
	return EXIT_SUCCESS;
}