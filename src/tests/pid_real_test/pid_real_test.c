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
	if (argc < 6) {
		errno = EINVAL;
		err(EXIT_FAILURE, "Not enough arguments");
	}
	int lret;
	int period;
	sscanf(argv[1], "%d", &period);
	struct sched_attr attr = {
		.size = sizeof(struct sched_attr),
		.sched_policy = SCHED_DEADLINE,
		.sched_priority = 0,
		.sched_flags = 0,
		.sched_period = period,
		.sched_runtime = period,
		.sched_deadline = period
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
	//	.command = 500.0,
		.feedback = 0.0,
		.delta_t = 0.0,

		.deadband = 1.0,

	//	.p_gain = 0.185,
	//	.i_gain = 0.070,
	//	.d_gain = 0.000,

		.max_error = 100000.0,
		.max_i_error = 100000.0,
		.max_d_error = 1000.0,
		.max_output = 100.0,

		.previous_error = 0.0,
	};
	sscanf(argv[2], "%lf", &(pid.command));
	sscanf(argv[3], "%lf", &(pid.p_gain));
	sscanf(argv[4], "%lf", &(pid.i_gain));
	sscanf(argv[5], "%lf", &(pid.d_gain));

	struct timespec prev_time = {0}, cur_time = {0};
	int fb;
	clock_gettime(CLOCK_MONOTONIC, &prev_time);
	sched_yield();

	while (keep_running) {
		encoder_get_speed(board, 1, &fb);
		pid.feedback = (double) fb;

		clock_gettime(CLOCK_MONOTONIC, &cur_time);
		pid.delta_t = (cur_time.tv_sec - prev_time.tv_sec) + \
			(cur_time.tv_nsec - prev_time.tv_nsec) / 1000000000.0;

		do_calcs(&pid);

		motor_set_speed(board, 1, (float) get_output(&pid));
		prev_time = cur_time;

		printf("\e[1;1H\e[2J");		// Clear screen
		printf("Feedback:  %3.1f RPM\n", pid.feedback);
		printf("delta_t:   %3.9f s\n", pid.delta_t);
		printf("Error:     %3.6f RPM\n", pid.error);
		printf("ErrorI:    %3.6f RPM\n", pid.i_error);
		printf("ErrorD:    %3.6f RPM\n", pid.d_error);
		printf("PrevError: %3.6f\n", pid.previous_error);
		printf("Output:    %3.6f %%\n", pid.output);
		printf("OutputP:   %3.6f\n", pid.p_output);
		printf("OutputI:   %3.6f\n", pid.i_output);
		printf("OutputD:   %3.6f\n", pid.d_output);

		sched_yield();
	}

	motor_stop(board, 1);
	board_close(board);
	free((struct dfr_board*) board);

	return EXIT_SUCCESS;
}
