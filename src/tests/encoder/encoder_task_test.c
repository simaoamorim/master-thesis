#include <encoder.h>
#include <encoder_task.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define ENCODER_PPR 12.0
#define GEARBOX_RATIO 30.0

int main (int argc, char *argv[])
{
	int retval;
	struct encoder_task enc = {0};
	if (-1 == encoder_init(&enc.encoder, 0, 17, 18)) {
		perror("Failed to open encoder GPIO");
		return -1;
	}
	pthread_t tid;
	pthread_create(&tid, NULL, encoder_task, &enc);
	long count;
	double prev_revs = 0;
	double revs, velocity, delta_t;
	struct timespec cur_time, prev_time;
	clock_gettime(CLOCK_MONOTONIC, &prev_time);
	for (int i = 0; i < 1000; i++) {
		usleep(10000);
		clock_gettime(CLOCK_MONOTONIC, &cur_time);
		delta_t = (cur_time.tv_sec - prev_time.tv_sec) + \
			(cur_time.tv_nsec - prev_time.tv_nsec) / 1000000000.0;
		count = encoder_task_get_count(&enc);
		revs = apply_scale(count, ENCODER_PPR);
		velocity = (double) (revs - prev_revs) / delta_t; // RPS motor
		velocity = apply_scale(velocity, GEARBOX_RATIO); // RPS output
		prev_time = cur_time;
		prev_revs = revs;

		printf("Counter: %ld\n", count);
		printf("Revolutions: %f\n", revs);
		printf("Velocity: %f\n", velocity);
		printf("Delta t: %f\n", delta_t);
	}
	pthread_kill(tid, SIGINT);
	pthread_join(tid, (void *) &retval);
	printf("Return value: %d\n", retval);
	printf("Final stage: %d\n", enc.encoder.stage);
	printf("Counter: %ld\n", enc.encoder.count);
	encoder_end(&enc.encoder);
	return 0;
}