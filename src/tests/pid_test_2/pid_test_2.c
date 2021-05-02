#include <stdio.h>
#include <errno.h>
#include <sched.h>

#include <encoder.h>
#include <encoder_task.h>
#include <pid.h>
#include <dfr0592.h>

#define	ENC_PPR			12
#define	MOTOR_GEARBOX_RATIO	30
#define	CONTROL_PERIOD			10000	// microseconds

#define	FAIL(str)		{perror(str); retval = -1; goto end;}
#define	delta(t1, t2)		(t2.tv_sec - t1.tv_sec) + ((t2.tv_nsec - t1.tv_nsec) / 1000000000.0)

int keep_running = 1;

void print_help (char *argv[]);

void sighandler (int signum);

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
	long revs = 0, prev_revs = 0;
	double motor_velocity = 0.0, output_velocity = 0.0;

	// Check argument count
	if (argc != 6) {
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
	set_pwm_frequency(dfr_board, 5000);
	for (int i = 0; i < 2; i++) {
		if (0 != encoder_disable(dfr_board, i))
			FAIL("encoder_disable() failed");
		if (0 != motor_stop(dfr_board, i))
			FAIL("motor_stop() failed");
	}

	// Initialize PID
	sscanf(argv[1], "%lf", &pid_s.p_gain);
	sscanf(argv[2], "%lf", &pid_s.i_gain);
	sscanf(argv[3], "%lf", &pid_s.d_gain);
	sscanf(argv[4], "%lf", &pid_s.deadband);
	sscanf(argv[5], "%lf", &pid_s.command);

	// Initializer encoder interface
	if (-1 == encoder_init(&encoder_struct.encoder, 0, 17, 18))
		FAIL("Failed to initialize encoder GPIO");

	// Initialize encoder task thread
	pthread_t thread_id;
	pthread_create(&thread_id, NULL, encoder_task, &encoder_struct);


end:
	if (NULL != dfr_board)
		free((void *) dfr_board);
	return retval;
}

void print_help (char *argv[])
{
	printf("Usage:\n");
	printf("\t%s p_gain i_gain d_gain deadband command\n", argv[0]);
	printf("\n");
	printf("Arguments:\n");
	printf("\tp_gain:\t\tProportional gain\n");
	printf("\ti_gain:\t\tIntegral gain\n");
	printf("\td_gain:\t\tDerivative gain\n");
	printf("\tdeadband:\tDeadband value\n");
	printf("\tcommand:\tCommand value\n");
}

void sighandler (int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
	}
}
