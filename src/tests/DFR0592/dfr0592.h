#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

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
const struct dfr_board * board_init(int i2c_bus, int addr);

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