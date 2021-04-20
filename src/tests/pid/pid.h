

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
	
	// Computations
	double error;
	double i_error;
	double d_error;
	
	// Limits
	double max_error;
	double max_i_error;
	double max_d_error;
	double bias;
	double max_output;
	
	// Results
	double p_output;
	double i_output;
	double d_output;
	double output;
};