#include <stdio.h>
#include <errno.h>
#include <err.h>
#include <gpiod.h>
#include <string.h>

int main (int argc, char *argv[])
{
	char chip_path[] = "/dev/gpiochip0";
	struct gpiod_chip *chip = gpiod_chip_open(chip_path);
	if (NULL == chip) {
		err(EXIT_FAILURE, "Failed to open chip device");
	}

	printf("Chip name: %s\n", gpiod_chip_name(chip));
	printf("Chip label: %s\n", gpiod_chip_label(chip));
	printf("Chip num lines: %d\n", gpiod_chip_num_lines(chip));

	struct gpiod_line *line = gpiod_chip_get_line(chip, 5);
	struct gpiod_line *line2 = gpiod_chip_find_line(chip, "GPIO17");

	printf("Line 1: %s\n", gpiod_line_name(line));
	printf("Line 1 consumer: %s\n", gpiod_line_consumer(line));
	int line_dir = gpiod_line_direction(line);
	char dir[20];
	if (GPIOD_LINE_DIRECTION_INPUT == line_dir)
		strncpy(dir, "Input", 19);
	else if (GPIOD_LINE_DIRECTION_OUTPUT == line_dir)
		strncpy(dir, "Output", 19);
	printf("Line 1 direction: %s\n", dir);
	printf("Line 1 is used: %d\n", gpiod_line_is_used(line));
	printf("Reserving line 1...");
	int lret = gpiod_line_request_input(line, "MYSELF");
	if (-1 == lret) {
		err(EXIT_FAILURE, "Failed to request line");
	}
	printf("Line reserverd.\nNew consumer: %s\n", gpiod_line_consumer(line));
	printf("Line 2: %s\n", gpiod_line_name(line2));

	gpiod_line_release(line);
	gpiod_chip_close(chip);
	if (NULL == chip)
		printf("chip is NULL after close\n");
	puts("All OK");
	return 0;
}
