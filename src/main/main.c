#include <stdio.h>
#include <errno.h>
#include <sched.h>
#include <string.h>
#include <sys/poll.h>

#include <encoder_task.h>
#include <pid.h>
#include <dfr0592.h>
#include <p_v_calc.h>
#include <control.h>

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
	struct dfr_board dfr_board = NEW_DFR_BOARD;
	struct timespec first_time, cur_time;
	FILE *debug_file = NULL;
	int iter = 0;
	struct encoder_task encoder_struct = {0};
	struct pid_s pid_s = NEW_PID_T;
	struct p_v_task_s pv_task_s = NEW_P_V_TASK_S;
	struct control_s control_s = NEW_CONTROL_S;
	double tstamp;
	pthread_t encoder_thread_id = -1;
	pthread_t p_v_thread_id = -1;
	pthread_t control_thread_id = -1;
	pthread_attr_t pthread_attrs;
	int control_period;
	size_t inbuf_size = 80;
	char *inbuf = calloc(sizeof(char), inbuf_size);
	struct pollfd fds = {.fd = fileno(stdin), .events = POLLIN};
	int lret;
	double new_command = 0.0;

	// Check argument count
	if (argc != 11 && argc != 12) {
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
	lret = board_init(&dfr_board, 1, 0x10);
	if (0 != lret)
		FAIL("board_init(1, 0x10) failed");
	if (0 != board_set_mode(&dfr_board, DC))
		FAIL("board_set_mode(DC) failed");
	set_pwm_frequency(&dfr_board, 10000);
	for (int i = 1; i < 3; i++) {
		if (0 != encoder_disable(&dfr_board, i))
			FAIL("encoder_disable() failed");
		if (0 != motor_stop(&dfr_board, i))
			FAIL("motor_stop() failed");
	}

	// Initialize PID
	sscanf(argv[1], "%lf", &pid_s.p_gain);
	sscanf(argv[2], "%lf", &pid_s.i_gain);
	sscanf(argv[3], "%lf", &pid_s.d_gain);
	sscanf(argv[4], "%lf", &pid_s.deadband);
	sscanf(argv[5], "%d", &control_period);
	sscanf(argv[6], "%lf", &pid_s.command);
	sscanf(argv[7], "%ld", &pv_task_s.period);
	sscanf(argv[8], "%ld", &pv_task_s.encoder_ppr);
	sscanf(argv[9], "%lf", &pv_task_s.gearbox_ratio);
	sscanf(argv[10], "%d", &encoder_struct.period);

	pv_task_s.enc_task = &encoder_struct;
	control_s.dfr_board = &dfr_board;
	control_s.pid_vel = &pid_s;
	control_s.pv_s = &pv_task_s;
	control_s.period = control_period;

	// Initializer encoder interface
	if (-1 == encoder_init(&encoder_struct.encoder, 0, 17, 18))
		FAIL("Failed to initialize encoder GPIO");

	// Initialize encoder task thread
	pthread_attr_init(&pthread_attrs);
	pthread_attr_setinheritsched(&pthread_attrs, PTHREAD_INHERIT_SCHED);
	pthread_create(&encoder_thread_id, NULL, encoder_task, &encoder_struct);
	pthread_create(&p_v_thread_id, NULL, p_v_task, &pv_task_s);
	pthread_create(&control_thread_id, &pthread_attrs, control_task, &control_s);

	int policy;
	pthread_getschedparam(control_thread_id, &policy, &sched_param);
	if (SCHED_FIFO != policy)
		fputs("Control thread did NOT inherit RT_FIFO scheduler", stderr);


	if (SIG_ERR == signal(SIGINT, sighandler)) {
		perror("signal(SIGINT, sighandler) failed");
		retval = -1;
		goto end;
	}

	if (argc == 12) {
		debug_file = init_pid_debug(&pid_s, argv[11]);
		if (NULL != debug_file) {
			debug_append_iteration(&pid_s, debug_file, iter, 0.0);
		} else {
			char string[100];
			sprintf(string, "Failed to open debug file \"%s\": ", argv[11]);
			perror(string);
			puts("Continuing without debug output");
		}
	}

	printf("> ");
	p_v_enable_task(&pv_task_s);
	clock_gettime(CLOCK_MONOTONIC, &first_time);
	usleep(control_period);

	while (keep_running) {
		clock_gettime(CLOCK_MONOTONIC, &cur_time);
		// Check stdin for new command value
		lret = poll(&fds, 1, 0);
		if (0 < lret) {
			getline(&inbuf, &inbuf_size, stdin);
			if (0 == strcmp("q\n", inbuf))
				keep_running = 0;
			else if (0 == sscanf(inbuf, "%lf", &new_command))
				 puts("Huh?");
			else
				control_task_set_velocity_command(&control_s, new_command);
			printf("> ");
		} else if (0 > lret) {
			FAIL("poll() on STDIN failed");
		}

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
	printf("Stopping auxiliary threads... ");
	encoder_task_stop();
	p_v_task_stop();
	control_task_stop(&control_s);
	if (-1 != encoder_thread_id)
		pthread_join(encoder_thread_id, (void *) &retval);
	if (-1 != p_v_thread_id)
		pthread_join(p_v_thread_id, (void *) &retval);
	if (-1 != control_thread_id)
		pthread_join(control_thread_id, (void *) &retval);
	puts("OK");
	if (-1 != dfr_board.i2c_fd) {
		printf("Stopping motor... ");
		motor_stop(&dfr_board, 1);
		printf("OK\n");
	}
	puts("All done. Goodbye!");
	return retval;
}

void print_help (char *argv[])
{
	printf("Usage:\n");
	printf("  %s p_gain i_gain d_gain deadband period "\
		"command p_v_period encoder_ppr gbox_ratio [filename]\n", argv[0]);
	printf("\n");
	printf("Arguments:\n");
	printf("  p_gain:      Proportional gain\n");
	printf("  i_gain:      Integral gain\n");
	printf("  d_gain:      Derivative gain\n");
	printf("  deadband:    Deadband value\n");
	printf("  period:      Control period\n");
	printf("  command:     Command value\n");
	printf("  p_v_period:  Period to calculate pos and vel\n");
	printf("  encoder_ppr: Motor encoder PPR\n");
	printf("  gbox_ratio:  Motor gearbox ratio\n");
	printf("  enc_period:  Encoder I/O parse period\n");
	printf("  filename:    File name to output debug into [Optional]\n");
}
