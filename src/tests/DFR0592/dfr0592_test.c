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
	printf("fd = %d and addr = %d\n", b->i2c_fd, b->addr);
	if (0 > board_close(b)) {
		perror("board_close()");
		return errno;
	}
	free((void *) b);
	return 0;
}
