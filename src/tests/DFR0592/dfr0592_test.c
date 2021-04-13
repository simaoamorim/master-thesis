#include "dfr0592.h"
#include <stdio.h>
#include <errno.h>

int main(int argc, char **argv) {
	if(argc != 1) {
		printf("%s takes no arguments.\n", argv[0]);
		return 1;
	}
	const struct dfr_board *b = board_init(1, 0x10);
	if (NULL == b) {
		fprintf(stderr, "board_init(%d, %d) failed\n", 1, 16);
		return errno;
	}
	if (0 > board_set_mode(b, DC)) {
		perror("board_set_mode()");
		return errno;
	}
	if (0 > set_pwm_frequency(b, 200)) {
		perror("set_pwm_frequency()");
		return errno;
	}
	if (0 > encoder_enable(b, 1)) {
		perror("encoder_enable(1)");
		return errno;
	}
	if (0 > encoder_disable(b, 2)) {
		perror("encoder_disable(2)");
		return errno;
	}
	// Error expected
	if (0 == encoder_disable(b, 3)) {
		fprintf(stderr, "encoder_disable(3) should have failed, but didn't!");
		return -1;
	}
	puts("^^ This error is expected ^^");
	puts("Everything OK");
	if (0 > board_close(b)) {
		perror("board_close()");
		return errno;
	}
	free((void *) b);
	return 0;
}
