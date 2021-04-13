#include "dfr0592.h"
#include <stdio.h>
#include <errno.h>
#include <time.h>

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
		goto ret_err;
	}
	if (0 > set_pwm_frequency(b, 200)) {
		perror("set_pwm_frequency()");
		goto ret_err;
	}
	if (0 > encoder_enable(b, 1)) {
		perror("encoder_enable(1)");
		goto ret_err;
	}
	if (0 > encoder_disable(b, 2)) {
		perror("encoder_disable(2)");
		goto ret_err;
	}
	// Error expected
	if (0 == encoder_disable(b, 3)) {
		fprintf(stderr, "encoder_disable(3) should have failed, but didn't!");
		goto ret_err;
	}
	puts("^^ This error is expected ^^");
	if (0 > motor_set_speed(b, 1, CW, 50)) {
		perror("motor_set_speed(1, CW, 50)");
		goto ret_err;
	}
	sleep(1);
	if (0 > motor_stop(b, 1)) {
		perror("motor_stop(1)");
		goto ret_err;
	}
	puts("Everything OK");
	if (0 > board_close(b)) {
		perror("board_close()");
		goto ret_err;
	}
	free((void *) b);
	return 0;
ret_err:
	free((void *) b);
	return -1;
}
