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
	if (ioctl(fd, I2C_SLAVE, addr) < 0) {
		perror("ioctl (set slave address)");
		return NULL;
	}
	int pid = i2c_smbus_read_byte_data(fd, _REG_PID);
	int vid = i2c_smbus_read_byte_data(fd, _REG_PVD);
	if (pid != _REG_DEF_PID || vid != _REG_DEF_VID) {
		fprintf(stderr, "PID/VID does't match\n");
		fprintf(stderr, "PID is 0x%x, should be 0x%x\n", pid, _REG_DEF_PID);
		fprintf(stderr, "VID is 0x%x, should be 0x%x\n", vid, _REG_DEF_VID);
		errno = ENXIO;
		return NULL;
	}
	struct dfr_board *new_board = malloc(sizeof(struct dfr_board));
	struct dfr_board tmp = {
		.i2c_fd = fd,
		.addr = addr,
		.pid = pid,
		.vid = vid};
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

int board_set_mode(const struct dfr_board *board, enum modes mode)
{
	if (DC == mode) {
		i2c_smbus_write_byte_data(board->i2c_fd, _REG_CTRL_MODE, _CONTROL_MODE_DC_MOTOR);
	} else if (STEPPER == mode) {
		i2c_smbus_write_byte_data(board->i2c_fd, _REG_CTRL_MODE, _CONTROL_MODE_STEPPER);
	} else {
		errno = EINVAL;
		return -1;
	}
	if (i2c_smbus_read_byte_data(board->i2c_fd, _REG_CTRL_MODE) != mode) {
		errno = EIO;
		return -1;
	}
	return 0;
}
