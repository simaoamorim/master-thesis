#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscall.h>

#include <linux/sched.h>
#include <linux/sched/types.h>
#include <signal.h>
#include <time.h>

int sched_setattr(pid_t pid, const struct sched_attr *attr,
		unsigned int flags);
int sched_getattr(pid_t pid, struct sched_attr *attr,
		unsigned int size, unsigned int flags);
int sched_yield();
void sighandler(int signum);

int keep_running = 1;

int main(int argc, char *argv[])
{
	int lret;
	struct sched_attr attr = {
		.size = sizeof(struct sched_attr),
		.sched_policy = SCHED_DEADLINE,
		.sched_priority = 0,
		.sched_flags = 0,
		.sched_period = 1000000,
		.sched_runtime = 1000000,
		.sched_deadline = 1000000
	};
	struct sched_attr attr2 = {0};
	struct timespec prev_time = {0}, cur_time = {0};

	if (SIG_ERR == signal(SIGINT, sighandler)) {
		perror("signal()");
		goto exit_fail;
	}

	lret = sched_setattr(0, &attr, 0);
	if (EXIT_FAILURE == lret) {
		perror("Failed to set SCHED_DEADLINE");
		goto exit_fail;
	}
	lret = sched_getattr(0, &attr2, sizeof(struct sched_attr), 0);
	if (EXIT_SUCCESS == lret) {
		if (SCHED_DEADLINE == attr2.sched_policy) {
			puts("Deadline configured OK");
		} else {
			puts("Could not configure deadline");
			goto exit_fail;
		}
	}

	long delta, max = 0L, min = 1000000000L;
	puts("Press Ctrl+C to stop...");
	clock_gettime(CLOCK_MONOTONIC, &prev_time);
	lret = sched_yield();
	if (EXIT_FAILURE == lret) {
		perror("sched_yield()");
		goto exit_fail;
	}
	while (keep_running) {
		clock_gettime(CLOCK_MONOTONIC, &cur_time);
		delta = cur_time.tv_nsec - prev_time.tv_nsec;
		if (delta < 0)
			delta += 1000000000UL;
		if (delta < min)
			min = delta;
		else if (delta > max)
			max = delta;
		prev_time = cur_time;
		sched_yield();
	}

	printf("Min: %luns\n", min);
	printf("Max: %luns\n", max);
	printf("Jitter: %luns\n", max - min);

	return EXIT_SUCCESS;
exit_fail:
	return EXIT_FAILURE;
}

int sched_setattr(pid_t pid,
		const struct sched_attr *attr,
		unsigned int flags)
{
	return syscall(__NR_sched_setattr, pid, attr, flags);
}

int sched_getattr(pid_t pid,
		struct sched_attr *attr, 
		unsigned int size,
		unsigned int flags)
{
	return syscall(__NR_sched_getattr, pid, attr, size, flags);
}

void sighandler(int signum)
{
	if (SIGINT == signum) {
		keep_running = 0;
		write(1, "\n", 1);
	}
	return;
}

int sched_yield()
{
	return syscall(__NR_sched_yield);
}

