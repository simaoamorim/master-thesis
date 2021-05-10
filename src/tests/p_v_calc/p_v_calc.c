#include "p_v_calc.h"

long encoder_ppr = 0;
double gearbox_ratio = 0.0;

void set_encoder_ppr (long ppr)
{
	encoder_ppr = ppr;
}

void set_gearbox_ratio (double ratio)
{
	gearbox_ratio = ratio;
}

int calc_velocity (long enc_count, double delta_t, double *output_w)
{
	static double prev_revs = 0.0;

	if (0 == encoder_ppr || 0.0 == gearbox_ratio)
		return -1;

	double revs = (double) enc_count / encoder_ppr;
	double motor_w = (revs - prev_revs) / delta_t;
	*output_w = motor_w / gearbox_ratio * 60.0;

	prev_revs = revs;

	return 0;
}
