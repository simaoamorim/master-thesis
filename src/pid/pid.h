#ifndef PID_H_
#define PID_H_

#include <stdio.h>

#define NEW_PID_T {\
	.command = 0.0,\
	.feedback = 0.0,\
	.delta_t = 0.0,\
\
	.p_gain = 0.0,\
	.d_gain = 0.0,\
	.deadband = 0.0,\
\
	.max_error = 1000000.0,\
	.max_i_error = 1000000.0,\
	.max_d_error = 1000000.0,\
	.max_output = 100.0,\
	.max_output_delta = 0.0,\
\
	.error = 0.0,\
	.previous_error = 0.0,\
	.i_error = 0.0,\
	.d_error = 0.0,\
\
	.p_output = 0.0,\
	.i_output = 0.0,\
	.d_output = 0.0,\
	.output = 0.0\
}

struct pid_s
{
	// Inputs
	double command;		///< Desired set-point
	double feedback;	///< Reading value
	double delta_t;		///< Time delta since last execution
	
	// Parameters
	double p_gain;		///< Proportional term
	double i_gain;		///< Integral term
	double d_gain;		///< Derivative term
	double deadband;	///< Absolute value to still assume a null error
	
	// Limits
	double max_error;
	double max_i_error;
	double max_d_error;
	double max_output;
	double max_output_delta;
	
	// Internal computations
	double error;
	double previous_error;
	double i_error;
	double d_error;
	
	// Results
	double p_output;
	double i_output;
	double d_output;
	double output;
};

void do_calcs (struct pid_s *p);

double get_output (struct pid_s *p);

FILE * init_pid_debug (struct pid_s *p, char filename[]);

void debug_append_iteration (struct pid_s *p, FILE *f, long iter, double tstamp);

#endif
