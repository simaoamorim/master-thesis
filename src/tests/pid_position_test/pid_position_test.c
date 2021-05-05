#include <stdio.h>
#include <errno.h>
#include <sched.h>
#include <string.h>
#include <sys/poll.h>

#include <encoder.h>
#include <encoder_task.h>
#include <pid.h>
#include <dfr0592.h>

#define	ENC_PPR			12.0
#define	MOTOR_GEARBOX_RATIO	30.0

#define	FAIL(str)		{perror(str); retval = -1; goto end;}
#define	delta(t1, t2)		(t2.tv_sec - t1.tv_sec) + ((t2.tv_nsec - t1.tv_nsec) / 1000000000.0)

volatile int keep_running = 1;

void print_help (char *argv[]);

void sighandler (int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
	}
}

int main (int argc, char *argv[])
{
	int retval = 0;
	struct dfr_board *dfr_board = NULL;
	struct timespec first_time, prev_time, cur_time;
	FILE *debug_file = NULL;
	int iter = 0;
	struct encoder_task encoder_struct = {0};
	struct pid_t pid_s = NEW_PID_T;
	double tstamp;
	long encoder_count = 0;
	pthread_t thread_id;
	int control_period;
	unsigned long inbuf_size = 80;
	char *inbuf = calloc(sizeof(char), inbuf_size);
	struct pollfd fds = {.fd = fileno(stdin), .events = POLLIN};
	int lret;

	// Check argument count
	if (argc != 7 && argc != 8) {
		if (argc != 1)
			fprintf(stderr, "Wrong usage\n\n");
		print_help(argv);
		goto end;
	}

	// Set real-time scheduler
	struct sched_param sched_param = {.sched_priority = 10};
	if (0 != sched_setscheduler(0, SCHED_FIFO, &sched_param))
		FAIL("Failed to create a realtime task");

	// Open connection with DFR0592 board
	dfr_board = (struct dfr_board *) board_init(1, 0x10);
	if (NULL == dfr_board)
		FAIL("board_init(1, 0x10) failed");
	if (0 != board_set_mode(dfr_board, DC))
		FAIL("board_set_mode(DC) failed");
	set_pwm_frequency(dfr_board, 10000);
	for (int i = 1; i < 3; i++) {
		if (0 != encoder_disable(dfr_board, i))
			FAIL("encoder_disable() failed");
		if (0 != motor_stop(dfr_board, i))
			FAIL("motor_stop() failed");
	}

	// Initialize PID
	pid_s.max_output = 50.0;
	sscanf(argv[1], "%lf", &pid_s.p_gain);
	sscanf(argv[2], "%lf", &pid_s.i_gain);
	sscanf(argv[3], "%lf", &pid_s.d_gain);
	sscanf(argv[4], "%lf", &pid_s.deadband);
	sscanf(argv[5], "%d", &control_period);
	sscanf(argv[6], "%lf", &pid_s.command);

	// Initializer encoder interface
	if (-1 == encoder_init(&encoder_struct.encoder, 0, 17, 18))
		FAIL("Failed to initialize encoder GPIO");

	// Initialize encoder task thread
	pthread_create(&thread_id, NULL, encoder_task, &encoder_struct);

	if (SIG_ERR == signal(SIGINT, sighandler)) {
		perror("signal(SIGINT, sighandler) failed");
		retval = -1;
		goto end;
	}

	if (argc == 8) {
		debug_file = init_pid_debug(&pid_s, argv[7]);
		if (NULL != debug_file) {
			debug_append_iteration(&pid_s, debug_file, iter, 0.0);
		} else {
			char string[100];
			sprintf(string, "Failed to open debug file \"%s\": ", argv[7]);
			perror(string);
			puts("Continuing without debug output");
		}
	}

	printf("> ");

	clock_gettime(CLOCK_MONOTONIC, &first_time);
	prev_time = first_time;
	usleep(control_period);

	while (keep_running) {
		clock_gettime(CLOCK_MONOTONIC, &cur_time);

		// Acquire inputs (including needed calculations)
		pid_s.delta_t = delta(prev_time, cur_time);
		encoder_count = encoder_task_get_count(&encoder_struct);
		pid_s.feedback = encoder_count;
		// Check stdin for new command value
		lret = poll(&fds, 1, 0);
		if (0 < lret) {
			getline(&inbuf, &inbuf_size, stdin);
			if (0 == strcmp("q\n", inbuf))
				keep_running = 0;
			else if (0 == sscanf(inbuf, "%lf", &pid_s.command))
				 puts("Huh?");
			printf("> ");
		} else if (0 > lret) {
			FAIL("poll() on STDIN failed");
		}

		// Execute computations
		do_calcs(&pid_s);

		// Update outputs
		motor_set_speed(dfr_board, 1, (float) get_output(&pid_s));

		// Update memories
		prev_time = cur_time;

		if (NULL != debug_file) {
			iter++;
			tstamp = delta(first_time, cur_time);
			debug_append_iteration(&pid_s, debug_file, iter, tstamp);
		}
		fflush(stdout);
		usleep(control_period);
	}

	putchar('\n');
	printf("Exited main loop\n");

end:
	if (NULL != dfr_board) {
		printf("Stopping motor...");
		motor_stop(dfr_board, 1);
		free((void *) dfr_board);
		printf("OK\n");
	}
	printf("Stopping auxiliary thread...");
	encoder_task_stop();
	pthread_join(thread_id, (void *) &retval);
	printf("OK\n");

	return retval;
}

void print_help (char *argv[])
{
	printf("Usage:\n");
	printf("\t%s p_gain i_gain d_gain deadband period command [debug_filename]\n", argv[0]);
	printf("\n");
	printf("Arguments:\n");
	printf("\tp_gain:\t\tProportional gain\n");
	printf("\ti_gain:\t\tIntegral gain\n");
	printf("\td_gain:\t\tDerivative gain\n");
	printf("\tdeadband:\tDeadband value\n");
	printf("\tperiod:\tControl period\n");
	printf("\tcommand:\tCommand value\n");
	printf("\tdebug_filename:\tFile name to output debug into [Optional]\n");
}
