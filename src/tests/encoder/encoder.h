#ifndef _QUAD_ENC_H_
#define _QUAD_ENC_H_

#include <errno.h>
#include <gpiod.h>

struct encoder {
	struct gpiod_chip *chip_handle;
	struct gpiod_line *a_line;
	struct gpiod_line *b_line;
	char stage;
	long count;
};

int encoder_init (struct encoder *e, int gpiochip, int pin_a, int pin_b);

int encoder_start (struct encoder *e);

#endif
