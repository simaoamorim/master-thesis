#ifndef COMM_H_
#define COMM_H_

#if defined(__arm__) || defined(__aarch64__)
#	include <cifx/cifxlinux.h>
#else
#	error Not on the apropriate platform
#endif

#include <stdio.h>
#include <stdlib.h>

#define	BUFFER_SIZE	512
#define CIFX_DEV	"cifX0"
#define	CIFX_INIT_DIR	NULL

#define NEW_COMM_S {\
	.spiport = NULL,\
	.driver = NULL,\
	.channel = NULL,\
}

struct comm_s {
	char *spiport;
	CIFXHANDLE *driver;
	CIFXHANDLE *channel;
};

#endif
