#include <stdio.h>

struct pid_t
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

void do_calcs (struct pid_t *p);

double get_output (struct pid_t *p);

FILE * init_pid_debug (struct pid_t *p, char filename[]);

void debug_append_iteration (struct pid_t *p, FILE *f, long iter, double tstamp);