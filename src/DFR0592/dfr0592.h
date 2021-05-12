#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#define	_STEPPER_COUNT	1
#define	_MOTOR_COUNT	2
#define NEW_DFR_BOARD {\
	.i2c_fd = -1,\
	.addr = -1,\
	.pid = -1,\
	.vid = -1,\
}

/// Board definition structure
struct dfr_board {
	int i2c_fd;	///< I2C bus file descriptor
	int addr;	///< Board slave address
	int pid;	///< Board PID
	int vid;	///< Board VID
};

/// Board mode definitions
enum modes {
	DC,		///< DC motor mode
	STEPPER		///< Stepper motor mode
};

/**
 * Initialize a new dfr_board struct
 * @param[in] i2c_bus Index of the I2C bus to use
 * @param[in] addr Slave address of the board
 *
 * Initialize a new dfr_board struct by opening a file descriptor of the
 * provided @i2c_bus and storing it in the dfr_board.i2c_bus variable. Also,
 * test if the desired slave is present on the bus, otherwise fail.
 *
 * Context: Run this to open a connection with the board, during program
 *          initialization. Note that you should run this only once, multiple
 *          calls will have undefined behaviour.
 * Return: Returns a dfr_board struct pointer, or NULL in case of error.
 *
 * In the event that an error occurs, errno will be set accordingly, so use
 * it to get the error cause.
 */
int board_init(struct dfr_board * new_board, int i2c_bus, int addr);

/**
 * Close existing connection to a board
 * @param[in] *board Board definition
 *
 * Closes the I2C bus file used to comunicate with the slave. No further
 * communication is possible and the *board paramenter should be freed afterwards.
 *
 * Return: 0 on success, -1 on failure.
 *
 * In the event of failure, errno is set accordingly, use it to get the cause.
 */
int board_close(const struct dfr_board *board);

/**
 * Set the board operation mode
 * @param[in] *board Board definition
 * @param[in] mode Operation mode, as defined in modes
 *
 * Defines the board operation mode depending on the type of motor used,
 * either DC motor or STEPPER motor.
 *
 * Return: 0 on success, -1 on failure
 */
int board_set_mode(const struct dfr_board *board, enum modes mode);

/**
 * Set the motor PWM frequency
 * @param[in] *board Board definition
 * @param[in] freq PWM frequency
 *
 * Defines the PWM frequency used to drive the motor(s). It should be a multiple of 50Hz and within
 * the range [100, 12750]Hz. In case the given value is within this range but it is not a multiple of
 * 50, then an approximation will be made to the closest valid value and a warning will be issued.
 *
 * Return: 0 on success, -1 on failure
 *
 * In the event of failure, use errno to get the cause.
 */
int set_pwm_frequency(const struct dfr_board *board, int freq);

/**
 * Enable an encoder
 * @param[in] *board Board definition
 * @param[ini] motor Motor number
 *
 * Enables the usage of the encoder for the provided motor number. Motor number needs to be within
 * [1, _MOTOR_COUNT].
 *
 * Return: 0 on success, -1 on failure
 *
 * In the event of failure, use errno to get the cause.
 */
int encoder_enable(const struct dfr_board *board, int motor);

/**
 * Disable an encoder
 * @param[in] *board Board definition
 * @param[in] motor Motor number
 *
 * Disables the usage of the encoder for the provided motor number. Motor number needs to be within
 * [1, _MOTOR_COUNT].
 *
 * Return: 0 on success, -1 on failure
 *
 * In the event of failure, use errno to get the cause.
 */
int encoder_disable(const struct dfr_board *board, int motor);

/**
 * Set the gearbox ratio of the motor
 * @param[in] *board Board definition
 * @param[in] motor Motor number
 * @param[in] ratio Gearbox ratio, as an integer
 *
 * The board firmware assumes the encoder has 16CPR resolution and allows to
 * set the gearbox ratio. Use this function to the that ratio as an integer nuber.
 * E.g. if the actual ratio is 43.8:1, set the ratio to 43.
 *
 * Return: 0 on success, -1 on failure
 *
 * In the event of failure, check errno to get the cause.
 */
int encoder_set_ratio(const struct dfr_board *board, int motor, int ratio);

/**
 * Get the RPM reading of the motor speed
 * @param[in] *board Board definition
 * @param[in] motor Motor number
 * @param[in] *speed Motor speed, in RPM
 *
 * The board calculates the motor RPM based on the encoder pulses. Use this function
 * to get that value written to the variable pointed to by *speed.
 *
 * Return: 0 on success, -1 on failure
 *
 * In the event of failure, check errno to get the cause.
 */
int encoder_get_speed(const struct dfr_board *board, int motor, int *speed);

/**
 * Set the motor speed percentage
 * @param[in] *board Board definition
 * @param[in] motor Motor number
 * @param[in] speed Motor speed (%) [-100.0, 100.0]
 *
 * The board uses PWM modulation to control the motor speed. Use this function
 * to set the Duty Cycle (DC) of the PWM wave used to drive the motor. Use positive
 * speed values for clockwise rotation and negative values for anti-clockwise roation.
 *
 * Return: 0 on success, -1 on failure
 *
 * In the event of failure, check errno to get the cause.
 */
int motor_set_speed(const struct dfr_board *board, int motor, float speed);

/*
 * Stop the motor movement
 * @param[in] *board Board definition
 * @param[in] motor Motor number
 *
 * This functions sets speed to 0 and sets a special value on the direction
 * register that indicates the motor should be disabled.
 *
 * Return: 0 on success, -1 on failure
 *
 * In the event of failure, check errno to get the cause.
 */
int motor_stop(const struct dfr_board *board, int motor);
