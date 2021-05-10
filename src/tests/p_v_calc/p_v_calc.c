#include "p_v_calc.h"

#define	OUTPUT_PPR	(encoder_ppr * gearbox_ratio)

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
	static double revs = 0.0;
	if (0.0 == OUTPUT_PPR)
		return -1;

	revs = (double) enc_count / OUTPUT_PPR;
	*output_w = (revs - prev_revs) / delta_t * 60.0;

	prev_revs = revs;

	return 0;
}

int calc_position (long enc_count, double *output_p)
{
	if (0.0 == OUTPUT_PPR)
		return -1;

	*output_p = (double) enc_count * 360.0 / OUTPUT_PPR;

	return 0;
}
