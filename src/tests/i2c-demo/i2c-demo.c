/*
 * i2c-demo.c
 *
 * Requires libi2c-dev to be installed and linked using the LDFLAG "-li2c"
 *
 */

#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#include <linux/i2c-dev.h>
#include <i2c/smbus.h>

#define _REG_PID 0x01
#define _REG_PVD 0x02

void pusage(char *argv[]);

int main(int argc, char *argv[])
{
	int file;
	int adapter_nr;
	int slave_addr;
	char filename[20];

	if (argc == 3) {
		sscanf(argv[1], "%d", &adapter_nr);
		sscanf(argv[2], "%x", &slave_addr);
	} else {
		errno = EINVAL;
		perror(NULL);
		pusage(argv);
		goto exit_err;
	}

	snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
	file = open(filename, I2C_RDWR);
	if (file < 0) {
		perror(filename);
		goto exit_err;
	}
	if (ioctl(file, I2C_SLAVE, slave_addr) < 0) {
		perror("ioctl (set slave address)");
		goto exit_err;
	}

	printf("%s opened successfully\n", filename);
	printf("Slave address set to %02x\n", slave_addr);

	int data;
	data = i2c_smbus_read_byte_data(file, _REG_PID);
	if (0 > data) {
		perror("i2c_smbus_read_byte_data");
		goto exit_err;
	}
	printf("PID: %d (%2x)\n", data, data);
	data = i2c_smbus_read_byte_data(file, _REG_PVD);
	if (0 > data) {
		perror("i2c_smbus_read_byte_data");
		goto exit_err;
	}
	printf("PVD: %d (%2x)\n", data, data);

	if (file > 0)
		close(file);
	return 0;
exit_err:
	if (file > 0)
		close(file);
	return -1;
}

void pusage(char *argv[])
{
	puts("Usage:");
	printf("\t%s adapter_nr slave_addr", argv[0]);
	puts("");
	return;
}
