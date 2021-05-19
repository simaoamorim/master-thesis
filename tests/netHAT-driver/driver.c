#include <stdio.h>
#include <cifx/cifxlinux.h>

#define BUFFER_SIZE 256
#define CIFX_DEV "cifX0"

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

int main (int argc, char *argv[])
{
	CIFXHANDLE driver;
	CIFXHANDLE channel;
	struct CIFX_LINUX_INIT init =
	{
		.init_options        = CIFX_DRIVER_INIT_AUTOSCAN,
		.iCardNumber         = 0,
		.fEnableCardLocking  = 0,
		.base_dir            = NULL,
		.poll_interval       = 0,
		.poll_StackSize      = 0,
		.trace_level         = 255,
		.user_card_cnt       = 0,
		.user_cards          = NULL,
	};
	int lret;

	lret = cifXDriverInit(&init);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not initialize the driver", lret);
		goto exit_error;
	}

	lret = xDriverOpen(&driver);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open driver", lret);
		goto exit_error;
	}

	lret = xChannelOpen(&driver, CIFX_DEV, 0, &channel);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open channel", lret);
		goto exit_error;
	}

	

	return 0;
exit_error:
	return -1;
}
