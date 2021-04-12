#include "dfr0592.h"
#include "dfr0592_addrs.h"

const struct dfr_board * board_init(int i2c_bus, int addr)
{
	char i2c_filename[17];
	snprintf(i2c_filename, 16, "/dev/i2c-%d", i2c_bus);
	int fd = open(i2c_filename, O_RDWR);
	if (fd < 0) {
		fprintf(stderr, "Failed to open I2C bus\n");
		perror(i2c_filename);
		return NULL;
	}
	// TODO: add code to check slave presence
	struct dfr_board *new_board = malloc(sizeof(struct dfr_board));
	struct dfr_board tmp = {.i2c_fd = fd, .addr = addr};
	*new_board = tmp;
	return new_board;
}

int board_close(const struct dfr_board *board)
{
	int tmp = close(board->i2c_fd);
	if (0 > tmp) {
		fprintf(stderr, "Failed to close file descriptor %d, ", board->i2c_fd);
		perror(NULL);
	}
	return tmp;
}
