#include "encoder.h"

int encoder_init (struct encoder *e, int gpiochip, int pin_a, int pin_b)
{
	e->chip_handle = gpiod_chip_open_by_number(gpiochip);
	if (NULL == e->chip_handle)
		return -1;
	e->a_line = gpiod_chip_get_line(e->chip_handle, pin_a);
	e->b_line = gpiod_chip_get_line(e->chip_handle, pin_b);
	e->stage = -1;
	e->count = 0;
	if (NULL == e->a_line || NULL == e->b_line)
		return -1;
	return 0;
}

int encoder_start (struct encoder *e)
{
	if (-1 == gpiod_line_request_both_edges_events(e->a_line, "ENCODER") || \
	    -1 == gpiod_line_request_both_edges_events(e->b_line, "ENCODER"))
		return -1;
	return 0;
}
