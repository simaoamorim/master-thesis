#include "pid.h"

void _calc_errors (struct pid_t *pid)
{
	static double previous_error = 0;
	
	// Proportional error
	pid->error = pid->command - pid->feedback;
	if (pid->error < pid->deadband && pid->error > -(pid->deadband))
		pid->error = 0;
	else if (pid->error > pid->max_error)
		pid->error = pid->max_error;
	else if (pid->error < -(pid->max_error))
		pid->error = -(pid->max_error);
	
	// Integral error
	pid->i_error = pid->error * pid->delta_t;
	if (pid->i_error > pid->max_i_error)
		pid->i_error = pid->max_i_error;
	else if (pid->i_error < -(pid->max_i_error))
		pid->i_error = -(pid->max_i_error);
	
	// Derivative error
	pid->d_error = (pid->error - previous_error) / pid->delta_t;
	if (pid->d_error > pid->max_d_error)
		pid->d_error = pid->max_d_error;
	else if (pid->d_error < -(pid->max_d_error))
		pid->d_error = -(pid->max_d_error);
	
	return;
}