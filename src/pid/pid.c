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

void _calc_errors (struct pid_s *p)
{
	p->previous2_error = p->previous_error;
	p->previous_error = p->error;
	p->error = (double) (p->command - p->feedback);

	// Proportional error
	if (p->form == POSITION)
		p->p_error = p->error;
	else if (p->form == VELOCITY)
		p->p_error = (double) (p->error - p->previous_error);
	_apply_deadband(&(p->p_error), p->deadband);
	if (0.0 != p->max_error)
		_apply_limit(&(p->p_error), p->max_error);

	// Integral error
	if (p->form == POSITION)
		p->i_error += p->error * p->delta_t;
	else if (p->form == VELOCITY)
		p->i_error = p->error * p->delta_t;
	if (0.0 != p->max_i_error)
		_apply_limit(&(p->i_error), p->max_i_error);

	// Derivative error
	if (p->form == POSITION)
		p->d_error = (double)((p->error - p->previous_error) / p->delta_t);
	else if (p->form == VELOCITY)
		p->d_error = (double)((p->error - (2 * p->previous_error) + p->previous2_error) / p->delta_t);
	if (0.0 != p->max_d_error)
		_apply_limit(&(p->d_error), p->max_d_error);

}

void _calc_internal_outputs (struct pid_s *p)
{
	// Proportional
	p->p_output = p->p_gain * p->p_error;
	// Integral
	p->i_output = p->i_gain * p->i_error;
	// Derivative
	p->d_output = p->d_gain * p->d_error;
}

void _calc_output (struct pid_s *p)
{
	static double new_output;
	if (p->form == POSITION)
		new_output = p->p_output + p->i_output + p->d_output;
	else if (p->form == VELOCITY)
		new_output = p->output + p->p_output + p->i_output + p->d_output;
	if (0.0 != p->max_output)
		_apply_limit(&new_output, p->max_output);
	if (0.0 != p->max_output_delta) {
		static double output_delta = 0;
		output_delta = new_output - p->output;
		_apply_limit(&output_delta, p->max_output_delta);
		new_output = p->output + output_delta;
	}
	p->output = new_output;
}

FILE * _create_csv (char filename[])
{
	return fopen(filename, "w");
}

void _write_header (struct pid_s *p, FILE *f)
{
	// Ensure beginning of the file
	rewind(f);
	// Write PID parameters and constants
	fprintf(f, "P_Gain,I_Gain,D_Gain,Deadband,max_error,max_i_error,max_d_error,max_output,,,,,,\n");
	fprintf(f, "%f,%f,%f,%f,%f,%f,%f,%f,,,,,,\n", \
		p->p_gain, p->i_gain, p->d_gain, p->deadband, p->max_error, p->max_i_error, \
		p->max_d_error, p->max_output);
	fprintf(f, "\n");
	// Write main table header
	fprintf(f, "N,Timestamp,Command,Feedback,delta_t,error,previous_error,p_error,i_error,d_error," \
		"p_output,i_output,d_output,output\n");
}

FILE * init_pid_debug (struct pid_s *p, char filename[])
{
	FILE * tmp = _create_csv(filename);
	if (NULL != tmp)
		_write_header(p, tmp);
	return tmp;
}

void do_calcs (struct pid_s *p)
{
	_calc_errors(p);
	_calc_internal_outputs(p);
	_calc_output(p);
}

double get_output (struct pid_s *p)
{
	return p->output;
}

void debug_append_iteration (struct pid_s *p, FILE *f, long iter, double tstamp)
{
	fprintf(f, "%ld,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf\n", \
		iter, tstamp, p->command, p->feedback, p->delta_t, p->error, p->previous_error, \
		p->p_error, p->i_error, p->d_error, p->p_output, p->i_output, p->d_output, p->output);
}
