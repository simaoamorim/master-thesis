#ifndef P_V_CALC_H_
#define P_V_CALC_H_

#include <sched.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <encoder_task.h>

#define NEW_P_V_TASK_S	{\
	.period = 1000,\
	.priority = 10,\
	.enc_task = NULL,\
	.velocity_mutex = NULL,\
	.position_mutex = NULL,\
	.encoder_ppr = 1,\
	.gearbox_ratio = 1.0,\
	.output_w = 0.0,\
	.output_p = 0.0,\
	.enabled = false,\
}

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
	bool enabled;
};

void set_encoder_ppr (struct p_v_task_s *p_v_task, long ppr);

void set_gearbox_ratio (struct p_v_task_s *p_v_task, double ratio);

int calc_velocity (struct p_v_task_s *p_v_task, long enc_count, double delta_t);

int calc_position (struct p_v_task_s *p_v_task, long enc_count);

#endif
