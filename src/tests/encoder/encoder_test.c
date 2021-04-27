#include <stdio.h>
#include <errno.h>

#include "encoder.h"

int main (int argc, char *argv[])
{
	struct encoder enc;
	if (-1 == encoder_init(&enc, 0, 17, 18)) {
		perror("Failed to initialize 'encoder'");
		return -1;
	}
	puts("Encoder open OK");
	return 0;
}
