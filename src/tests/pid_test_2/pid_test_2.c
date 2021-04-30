#include <stdio.h>
#include <errno.h>
#include <sched.h>

#include <encoder.h>
#include <encoder_task.h>
#include <pid.h>
#include <dfr0592.h>

#define	FAIL(str)	{perror(str); retval = -1; goto end;}

int keep_running = 1;

void sighandler (int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
	}
}

int main (int argc, char *argv[])
{
	int retval;

	// Set real-time scheduler
	struct sched_param sched_param = {.sched_priority = 10};
	if (0 != sched_setscheduler(0, SCHED_FIFO, &sched_param))
		FAIL("Failed to create a realtime task");

	// Open connection with DFR0592 board
	const struct dfr_board *dfr_board = board_init(1, 0x10);
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

	// Initializer encoder interface
	struct encoder_task encoder_struct = {0};
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
