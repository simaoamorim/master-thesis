#include "encoder.h"

int encoder_init (struct encoder *e, int gpiochip, int pin_a, int pin_b)
{
	e->chip_handle = gpiod_chip_open_by_number(gpiochip);
	if (NULL == e->chip_handle)
		return -1;
	e->a_line = gpiod_chip_get_line(e->chip_handle, pin_a);
	e->b_line = gpiod_chip_get_line(e->chip_handle, pin_b);
	if (NULL == e->a_line || NULL == e->b_line)
		return -1;
	e->stage = -1;
	e->count = 0;
	e->inputs = (struct gpiod_line_bulk *) malloc(sizeof(*e->inputs));
	gpiod_line_bulk_init(e->inputs);
	gpiod_line_bulk_add(e->inputs, e->a_line);
	gpiod_line_bulk_add(e->inputs, e->b_line);
	gpiod_line_request_bulk_input(e->inputs, CONSUMER_NAME);
	return 0;
}

int encoder_end (struct encoder *e)
{
	if (NULL != e->inputs) {
		gpiod_line_release_bulk(e->inputs);
		free(e->inputs);
	} else if (NULL != e->a_line && NULL != e->b_line) {
		gpiod_line_release(e->a_line);
		gpiod_line_release(e->b_line);
	}
	e->inputs = NULL;
	e->a_line = NULL;
	e->b_line = NULL;
	if (NULL != e->chip_handle)
		gpiod_chip_close(e->chip_handle);
	e->chip_handle = NULL;
	return 0;
}
