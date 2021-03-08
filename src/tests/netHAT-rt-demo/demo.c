#include <stdio.h>
#include <stdlib.h>

#include <cifx/cifxlinux.h>

#define	BUFFER_SIZE	512

void _perror(const char *s);

void _xerror(const char *s, const int32_t lret);

int main (int argc, char *argv[])
{
	CIFXHANDLE driver = NULL;
	int32_t lret;

	lret = xDriverOpen(&driver);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Could not open driver", lret);
		goto exit_error;
	}

	// More code

	lret = xDriverClose(&driver);
	if (CIFX_NO_ERROR != lret) {
		_xerror("Failed to close the driver", lret);
		goto exit_error;
	}

	return EXIT_SUCCESS;
exit_error:
	return EXIT_FAILURE;
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
