#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <cifx/cifxlinux.h>

#include <linux/sched.h>
#include "sched.h"

#define	BUFFER_SIZE	512
#define CIFX_DEV	"cifX0"
#define	CIFX_INIT_DIR	"/dev/spidev0.0"

volatile int keep_running = 1;

void _perror(const char *s);

void _xerror(const char *s, const int32_t lret);

int _cifx_init(const char *spiport, CIFXHANDLE *driver, CIFXHANDLE *channel);
int _cifx_end(const CIFXHANDLE *driver, const CIFXHANDLE *channel);

void sighandler(int signum);

void loop(void);

int main (int argc, char *argv[])
{
	CIFXHANDLE driver = NULL;
	CIFXHANDLE channel = NULL;
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

	if (SIG_ERR == signal(SIGINT, sighandler)) {
		perror("Error setting signal handler");
		goto exit_error;
	}

	lret = _cifx_init(CIFX_INIT_DIR, &driver, &channel);
	if (EXIT_FAILURE == lret)
		goto exit_error;

	lret = sched_setattr(0, &attr, 0);
	if (-1 == lret) {
		perror("Failed to set the deadline scheduler");
		puts("Make sure you have enough privileges to run an RT application" \
			"either by using 'sudo' or other methods");
		goto skip_sched;
	}
	lret = sched_getattr(0, &attr, sizeof(struct sched_attr), 0);
	if (SCHED_DEADLINE != attr.sched_policy) {
		puts("Current scheduler is not the requested (SCHED_DEADLINE)");
		goto exit_error;
	}
skip_sched:
	puts("Driver successfully opened");
	uint32_t ulState;
	while (keep_running) {
		lret = xChannelBusState(channel, CIFX_BUS_STATE_ON, &ulState, 1000);
		if (CIFX_DEV_NO_COM_FLAG == lret) {
			puts("Waiting for communication...");
			goto skip;
		}
		loop();
	skip:
		sched_yield();
	}

	lret = _cifx_end(&driver, &channel);

	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
}

void loop(void)
{
	// Loop function code here
}

void sighandler(int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
		write(STDOUT_FILENO, "\n", 1);
	}
}

void _perror(const char *s)
{
	printf("ERROR: %s\n", s);
	return;
}

void _xerror(const char *s, const int32_t lret)
{
	_perror(s);
	char tmpbuf[BUFFER_SIZE] = {0};
	xDriverGetErrorDescription(lret, tmpbuf, BUFFER_SIZE);
	printf("Cause: %s\n", tmpbuf);
}

int _cifx_init(const char *spiport, CIFXHANDLE *driver, CIFXHANDLE *channel)
{
	int lret;
	struct CIFX_LINUX_INIT init =
	{
		.init_options        = CIFX_DRIVER_INIT_AUTOSCAN,
		.iCardNumber         = 0,
		.fEnableCardLocking  = 0,
		.base_dir            = spiport,
		.poll_interval       = 0,
		.poll_StackSize      = 0,
		.trace_level         = 255,
		.user_card_cnt       = 0,
		.user_cards          = NULL,
	};

	lret = cifXDriverInit(&init);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not initialize the driver", lret);
		goto exit_error;
	}

	lret = xDriverOpen(driver);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open driver", lret);
		goto exit_error;
	}

	lret = xChannelOpen(NULL, CIFX_DEV, 0, channel);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open channel", lret);
		goto exit_error;
	}

	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
}

int _cifx_end(const CIFXHANDLE *driver, const CIFXHANDLE *channel)
{
	int32_t lret;
	lret = xDriverClose(&driver);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Failed to close the driver", lret);
		goto exit_error;
	}

	cifXDriverDeinit();

	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
}
