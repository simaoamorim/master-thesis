#ifndef P_V_CALC_H_
#define P_V_CALC_H_

void set_encoder_ppr (long ppr);

void set_gearbox_ratio (double ratio);

double calc_velocity (long enc_count, double delta_t);

#endif
