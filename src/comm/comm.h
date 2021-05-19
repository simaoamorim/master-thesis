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
	.sendData = {0},\
	.recvData = {0},\
	.ulState = 0,\
	.timeout = 10,\
}

struct comm_s {
	char *spiport;
	CIFXHANDLE *driver;
	CIFXHANDLE *channel;
	uint8_t  sendData[32];
	uint8_t  recvData[32];
	uint32_t ulState;
	uint32_t timeout;
};

int comm_init (struct comm_s *cs);

void comm_bus_wait (struct comm_s *cs);

int comm_update_inputs (struct comm_s *cs);

int comm_update_outputs (struct comm_s *cs);

uint8_t comm_get_input_byte (struct comm_s *cs, int offset);

uint16_t comm_get_input_word (struct comm_s *cs, int offset);

#endif
