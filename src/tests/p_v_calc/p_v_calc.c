#include "p_v_calc.h"

long encoder_ppr = 0;
double gearbox_ratio = 0.0;

double calc_velocity (long enc_count, double delta_t)
{
	static double prev_revs = 0.0;

	double revs = (double) enc_count / encoder_ppr;
	double motor_w = (revs - prev_revs) / delta_t;
	double output_w = motor_w / gearbox_ratio * 60.0;

	prev_revs = revs;

	return output_w;
}
