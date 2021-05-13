#include "comm.h"

void _perror (const char *s)
{
	printf("ERROR: %s\n", s);
	return;
}

void _xerror (const char *s, const int32_t lret)
{
	_perror(s);
	char tmpbuf[BUFFER_SIZE] = {0};
	xDriverGetErrorDescription(lret, tmpbuf, BUFFER_SIZE);
	printf("Cause: %s\n", tmpbuf);
}

int _cifx_init (const char *spiport, CIFXHANDLE *driver, CIFXHANDLE *channel)
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

	lret = xChannelOpen(driver, CIFX_DEV, 0, channel);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open channel", lret);
		goto exit_error;
	}

	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
}

int _cifx_end (const CIFXHANDLE *driver, const CIFXHANDLE *channel)
{
	int lret;
	lret = xChannelClose(&channel);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Failed to close the channel", lret);
		goto exit_error;
	}
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


