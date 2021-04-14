#include "dfr0592.h"
#include "dfr0592_addrs.h"

#define	NULLBOARD	"struct dfr_board *board is NULL\n"
#define	INVALIDMOTOR	"motor %d is invalid: needs to be within [%d,%d]\n", motor, 1, _MOTOR_COUNT

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

int set_pwm_frequency(const struct dfr_board *board, int freq)
{
	if (freq < 100 || freq > 12750) {
		fprintf(stderr, "Requested frequency out of range [100,12750]Hz\n");
		errno = EINVAL;
		return -1;
	}
	int tmp_freq = (int) freq / 50;
	if (0 != freq % 50) {
		fprintf(stderr, "Warning: actual frequency set to the closest multiple of 50Hz\n");
		fprintf(stderr, "Frequency set to %dHz\n", tmp_freq * 50);
	}
	int lret = i2c_smbus_write_byte_data(board->i2c_fd, _REG_MOTOR_PWM, tmp_freq);
	int rd_freq = i2c_smbus_read_byte_data(board->i2c_fd, _REG_MOTOR_PWM);
	if (0 != lret || rd_freq < 0 || rd_freq != tmp_freq) {
		errno = EIO;
		fprintf(stderr, "Failed to set frequency\n");
		return -1;
	}
	return 0;
}

int _encoder_set(const struct dfr_board *board, int motor, int value)
{
	if (NULL == board) {
		fprintf(stderr, "struct dfr_board *board is NULL\n");
		goto ret_inval;
	} else if ( _MOTOR_COUNT < motor || 1 > motor ) {
		fprintf(stderr, "motor %d is invalid: needs to be within [%d,%d]\n", motor, 1, _MOTOR_COUNT);
		goto ret_inval;
	}
	int reg = _REG_ENCODER1_EN + ((motor - 1)*0x05); // Use reg according to the provided motor
	i2c_smbus_write_byte_data(board->i2c_fd, reg, value);
	return 0;
ret_inval:
	errno = EINVAL;
	return -1;
}

int encoder_enable(const struct dfr_board *board, int motor)
{
	return _encoder_set(board, motor, 0x01);
}

int encoder_disable(const struct dfr_board *board, int motor)
{
	return _encoder_set(board, motor, 0x00);
}

int _motor_set_speed(const struct dfr_board *board, int motor, int orientation, int speed)
{
	if (NULL == board) {
		fprintf(stderr, NULLBOARD);
		goto ret_inval;
	}
	if (motor < 1 || motor > _MOTOR_COUNT) {
		fprintf(stderr, INVALIDMOTOR);
		goto ret_inval;
	}
	if (!(CW == orientation || CCW == orientation || STOP == orientation)) {
		fprintf(stderr, "Invalid orientation (%d)\n", orientation);
		goto ret_inval;
	}
	if (speed > 100 || speed < 0) {
		fprintf(stderr, "Invalid speed (%d)\n", speed);
		goto ret_inval;
	}
	motor--;
	int oreg = _REG_MOTOR1_ORIENTATION + (motor * 0x03);
	int sreg = _REG_MOTOR1_SPEED + (motor * 0x03);
	i2c_smbus_write_byte_data(board->i2c_fd, oreg, orientation);
	i2c_smbus_write_byte_data(board->i2c_fd, sreg, speed);
	i2c_smbus_write_byte_data(board->i2c_fd, sreg+1, 0);
	return 0;
ret_inval:
	errno = EINVAL;
	return -1;
}

int motor_set_speed(const struct dfr_board *board, int motor, int speed)
{
	if (0 > speed)
		speed = -speed;
	int orientation = speed >= 0 ? CCW : CW;
	return _motor_set_speed(board, motor, orientation, speed);
}

int motor_stop(const struct dfr_board *board, int motor)
{
	return _motor_set_speed(board, motor, STOP, 0);
}
