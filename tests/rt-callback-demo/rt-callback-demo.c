#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sched.h>
#include <signal.h>
#include <time.h>

struct stats {
	unsigned long max;
	unsigned long min;
	unsigned long jitter;
};

void periodic_func(union sigval sigev_value);

void sighandler(int signum);

int main(int argc, char *argv[])
{
	timer_t timerid;
	int lret;
	struct itimerspec tspec = {
		.it_interval.tv_sec = 0,
		.it_interval.tv_nsec = 1000000,
		.it_value.tv_sec = 0,
		.it_value.tv_nsec = 1000000
	};
	struct itimerspec tspec_stop = {0};
	struct sched_param sched_param = {.sched_priority = 10};
	struct stats stats = {
		.max = 0,
		.min = 1000000000UL,
		.jitter = 0
	};
	union sigval sigev_value = {
		.sival_ptr = &stats
	};
	struct sigevent sevp = {
		.sigev_notify = SIGEV_THREAD,
		.sigev_value = sigev_value,
		.sigev_notify_function = periodic_func
	};

	lret = sched_setscheduler(0, SCHED_FIFO, &sched_param);
	if (EXIT_FAILURE == lret) {
		perror("Failed to set FIFO scheduler");
		goto exit_failure;
	}

	lret = timer_create(CLOCK_MONOTONIC, &sevp, &timerid);
	if (EXIT_FAILURE == lret) {
		perror("Failed to create new timer");
		goto exit_failure;
	}

	timer_settime(timerid, 0, &tspec, NULL);

	puts("Press return to exit...");
	getchar();

	timer_settime(timerid, 0, &tspec_stop, NULL);

	stats.jitter = stats.max - stats.min;
	printf("Min: %luns\n", stats.min);
	printf("Max: %luns\n", stats.max);
	printf("Jitter: %luns\n", stats.jitter);

	return EXIT_SUCCESS;
exit_failure:
	return EXIT_FAILURE;
}

void periodic_func(union sigval sigev_value)
{
	static struct timespec prev_time = {0}, cur_time = {0};
	static long delta;
	struct stats *stats = sigev_value.sival_ptr;
	unsigned long *min = &(stats->min);
	unsigned long *max = &(stats->max);

	clock_gettime(CLOCK_MONOTONIC, &cur_time);
	if (0 == prev_time.tv_nsec)
		goto finish;

	delta = cur_time.tv_nsec - prev_time.tv_nsec;
	if (delta < 0)
		delta += 1000000000UL;
	if (delta < *min)
		*min = delta;
	else if (delta > *max)
		*max = delta;
finish:
	prev_time = cur_time;
	return;
}

