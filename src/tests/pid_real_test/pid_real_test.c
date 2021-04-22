#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <err.h>
#include <string.h>
#include <ctype.h>
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

/*
 * [optional]
 *
 * ./pid_real_test period command p_gain i_gain d_gain [filename]
 *
 *	period : Control period in nanoseconds (ns)
 *	command : commanded speed in RPM
 *	p_gain : Proportional gain to use in the PID
 *	i_gain : Integral gain to use in the PID
 *	d_gain : Derivative gain to use in the PID
 *	filename : Export PID debug to file "filename"
 *
 */
int main (int argc, char *argv[])
{
	if (argc < 6) {
		errno = EINVAL;
		err(EXIT_FAILURE, "Not enough arguments");
	}
	int use_debug = 0;
	char *d_filename = NULL;
	if (argc == 7) {
		use_debug = 1;
		d_filename = strdup(argv[6]);
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
		.feedback = 0.0,
		.delta_t = 0.0,

		.deadband = 1.0,

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

	FILE *d_file = NULL;
	long iter = 0;
	double tstamp;
	struct timespec first_time, prev_time, cur_time;
	int fb;

	if (use_debug) {
		d_file = init_pid_debug(&pid, d_filename);
		if (NULL != d_file) {
			debug_append_iteration(&pid, d_file, iter, 0.0);
		} else {
			fprintf(stderr, "Failed to open debug file \"%s\": ", d_filename);
			perror(NULL);
			use_debug = 0;
			puts("Continuing without debug output");
		}
		free(d_filename);
	}

	clock_gettime(CLOCK_MONOTONIC, &prev_time);
	first_time = prev_time;

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

		if (use_debug) {
			iter++;
			tstamp = (cur_time.tv_sec - first_time.tv_sec) + \
				(cur_time.tv_nsec - first_time.tv_nsec) / 1000000000.0;
			debug_append_iteration(&pid, d_file, iter, tstamp);
		}

		sched_yield();
	}

	puts("Exiting now...");
	motor_stop(board, 1);
	board_close(board);
	if (NULL != d_file) {
		fflush(d_file);
		fclose(d_file);
	}
	free((struct dfr_board*) board);

	return EXIT_SUCCESS;
}
