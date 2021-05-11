#ifndef P_V_CALC_H_
#define P_V_CALC_H_

#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <encoder_task.h>

struct p_v_task_s {
	long period;
	int priority;
	struct encoder_task *enc_task;
	pthread_mutex_t velocity_mutex;
	pthread_mutex_t position_mutex;
	long encoder_ppr;
	double gearbox_ratio;
	double output_w;
	double output_p;
};

void set_encoder_ppr (struct p_v_task_s *p_v_task, long ppr);

void set_gearbox_ratio (struct p_v_task_s *p_v_task, double ratio);

int calc_velocity (struct p_v_task_s *p_v_task, long enc_count, double delta_t);

int calc_position (struct p_v_task_s *p_v_task, long enc_count);

#endif
