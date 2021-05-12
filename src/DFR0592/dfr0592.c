#include "dfr0592.h"
#include "dfr0592_addrs.h"

#define	NULLBOARD	"struct dfr_board *board is NULL\n"
#define	INVALIDMOTOR	"motor %d is invalid: needs to be within [%d,%d]\n", motor, 1, _MOTOR_COUNT

int board_init(struct dfr_board * new_board, int i2c_bus, int addr)
{
	char i2c_filename[17];
	snprintf(i2c_filename, 16, "/dev/i2c-%d", i2c_bus);
	int fd = open(i2c_filename, O_RDWR);
	if (fd < 0 || ioctl(fd, I2C_SLAVE, addr) < 0)
		return -1;
	int pid = i2c_smbus_read_byte_data(fd, _REG_PID);
	int vid = i2c_smbus_read_byte_data(fd, _REG_PVD);
	if (pid != _REG_DEF_PID || vid != _REG_DEF_VID) {
		fprintf(stderr, "PID or VID does't match\n");
		fprintf(stderr, "PID is 0x%x, should be 0x%x\n", pid, _REG_DEF_PID);
		fprintf(stderr, "VID is 0x%x, should be 0x%x\n", vid, _REG_DEF_VID);
		errno = ENXIO;
		return -1;
	}
	new_board->i2c_fd = fd;
	new_board->addr = addr;
	new_board->pid = pid;
	new_board->vid = vid;
	return 0;
}

int board_close(const struct dfr_board *board)
{
	return close(board->i2c_fd);
}

int board_set_mode(const struct dfr_board *board, enum modes mode)
{
	if (DC == mode)
		return i2c_smbus_write_byte_data(board->i2c_fd, _REG_CTRL_MODE, _CONTROL_MODE_DC_MOTOR);
	else if (STEPPER == mode)
		return i2c_smbus_write_byte_data(board->i2c_fd, _REG_CTRL_MODE, _CONTROL_MODE_STEPPER);
	errno = EINVAL;
	return -1;
}

int set_pwm_frequency(const struct dfr_board *board, int freq)
{
	if (freq < 100 || freq > 12750) {
		errno = EINVAL;
		return -1;
	}
	return i2c_smbus_write_byte_data(board->i2c_fd, _REG_MOTOR_PWM, (int) freq / 50);
}

int _encoder_set(const struct dfr_board *board, int motor, int value)
{
	if (_MOTOR_COUNT < motor || 1 > motor) {
		errno = EINVAL;
		return -1;
	}
	int reg = _REG_ENCODER1_EN + ((motor-1) * 0x05);
	return i2c_smbus_write_byte_data(board->i2c_fd, reg, value);
}

int encoder_enable(const struct dfr_board *board, int motor)
{
	return _encoder_set(board, motor, 0x01);
}

int encoder_disable(const struct dfr_board *board, int motor)
{
	return _encoder_set(board, motor, 0x00);
}

int encoder_set_ratio(const struct dfr_board *board, int motor, int ratio)
{
	if (motor < 1 || motor > _MOTOR_COUNT || ratio < 1 || ratio > 2000)
		goto ret_inval;
	int reg = _REG_ENCODER1_REDUCTION_RATIO + ((motor-1) * 0x05);
	//unsigned char tmp[2] = {ratio & 0xFF, (ratio >> 8) & 0xFF};
	int tmp = ((ratio << 8) & 0xFF00) | ((ratio >> 8) & 0xFF);
	return i2c_smbus_write_word_data(board->i2c_fd, reg, tmp);
ret_inval:
	errno = EINVAL;
	return -1;
}

int encoder_get_speed(const struct dfr_board *board, int motor, int *speed)
{
	if (motor < 1 || motor > _MOTOR_COUNT || NULL == speed)
		goto ret_inval;
	int res = 0;
	int reg = _REG_ENCODER1_SPEED + ((motor-1) * 0x05);
	res = i2c_smbus_read_word_data(board->i2c_fd, reg);
	*speed = ((res << 8) & 0xFF00) | ((res >> 8) & 0xFF);
	if (*speed & 0x8000)
		*speed = - (0x10000 - *speed);
	return 0;
ret_inval:
	errno = EINVAL;
	return -1;
}

int _motor_set_speed(const struct dfr_board *board, int motor, int orientation, float speed)
{
	if (motor < 1 || motor > _MOTOR_COUNT || speed > 100.0 || speed < 0.0)
		goto ret_inval;
	static int or = STOP;
	int oreg = _REG_MOTOR1_ORIENTATION + ((motor-1) * 0x03);
	int sreg = _REG_MOTOR1_SPEED + ((motor-1) * 0x03);
	unsigned char tmp[2] = {(char) speed, (char) (speed*10)%10};
	if (or != orientation) {
		int lret = i2c_smbus_write_byte_data(board->i2c_fd, oreg, (char) orientation);
		if (0 > lret)
			return lret;
		or = orientation;
		if (STOP == or)
			return lret;
	}
	return i2c_smbus_write_word_data(board->i2c_fd, sreg, *tmp);
ret_inval:
	errno = EINVAL;
	return -1;
}

int motor_set_speed(const struct dfr_board *board, int motor, float speed)
{
	int orientation = speed >= 0.0 ? CCW : CW;
	if (0 > speed)
		speed = -speed;
	return _motor_set_speed(board, motor, orientation, speed);
}

int motor_stop(const struct dfr_board *board, int motor)
{
	return _motor_set_speed(board, motor, STOP, 0.0);
}
