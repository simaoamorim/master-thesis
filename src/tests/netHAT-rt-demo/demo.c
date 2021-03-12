#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include <cifx/cifxlinux.h>

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

int main (int argc, char *argv[])
{
	CIFXHANDLE driver = NULL;
	CIFXHANDLE channel = NULL;
	int lret;

	lret = _cifx_init(CIFX_INIT_DIR, &driver, &channel);
	if (EXIT_FAILURE == lret)
		goto exit_error;

	// More code
	puts("Driver successfully opened");

	lret = _cifx_end(&driver, &channel);

	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
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

int _cifx_init(	const char *spiport, CIFXHANDLE *driver, CIFXHANDLE *channel)
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
