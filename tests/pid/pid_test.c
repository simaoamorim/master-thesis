#include <stdio.h>
#include <stdlib.h>
#include <pid.h>

int main (int argc, char *argv[])
{
	struct pid_t pid = {
		.command = 10.0,
		.feedback = 0.0,
		.delta_t = 0.001139684,
		
		.p_gain = 2.0,
		.i_gain = 0.1,
		.d_gain = 0.001,
		.deadband = 0.0,
		
		.max_error = 100.0,
		.max_i_error = 100.0,
		.max_d_error = 100.0,
		.max_output = 100.0,
	};
	
	do_calcs(&pid);
	
	printf("Output %f\n", get_output(&pid));
	
	return EXIT_SUCCESS;
}