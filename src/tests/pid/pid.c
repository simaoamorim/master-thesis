#include "pid.h"

void _apply_limit (double *value, double limit)
{
	if (*value > limit)
		*value = limit;
	else if (*value < -limit)
		*value = -limit;
}

void _apply_deadband (double *value, double deadband)
{
	if (*value < deadband && *value > -deadband)
		*value = (double) 0.0;
}

void _calc_errors (struct pid_t *p)
{
	// Proportional error
	p->error = (double) p->command - p->feedback;
	_apply_deadband(&(p->error), p->deadband);
	_apply_limit(&(p->error), p->max_error);

	// Integral error
	p->i_error += p->error * p->delta_t;
	_apply_limit(&(p->i_error), p->max_i_error);

	// Derivative error
	p->d_error = ((double)(p->error - p->previous_error)) / p->delta_t;
	_apply_limit(&(p->d_error), p->max_d_error);

	p->previous_error = p->error;
}

void _calc_internal_outputs (struct pid_t *p)
{
	// Proportional
	p->p_output = p->p_gain * p->error;
	// Integral
	p->i_output = p->i_gain * p->i_error;
	// Derivative
	p->d_output = p->d_gain * p->d_error;
}

void _calc_output (struct pid_t *p)
{
	p->output = p->p_output + p->i_output + p->d_output;
	_apply_limit(&(p->output), p->max_output);
}

void do_calcs (struct pid_t *p)
{
	_calc_errors(p);
	_calc_internal_outputs(p);
	_calc_output(p);
}

double get_output (struct pid_t *p)
{
	return p->output;
}
