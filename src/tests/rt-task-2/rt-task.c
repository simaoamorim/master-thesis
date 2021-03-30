#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sched.h>

#define CLOCKID CLOCK_MONOTONIC
#define SIG SIGRTMIN

/********************/
/* Global Variables */
/********************/

volatile int keep_running = 1;

/******************/
/* Signal Handler */
/******************/

void sighandler(int signum) {
	if (SIGINT == signum) {
		keep_running = 0;
		putchar('\n');
	}
}

/********************/
/* Public Functions */
/********************/

int main(int argc, char *argv[])
{
        timer_t timerid;
        int retval;
        struct itimerspec tspec = {
        	.it_interval.tv_sec = 0,
        	.it_interval.tv_nsec = 1000000,
        	.it_value.tv_sec = 0,
        	.it_value.tv_nsec = 1000000
        };
        struct sched_param sched_param = {.sched_priority = 1};
        struct sigevent sev;
        sigset_t sigset;
        sigemptyset(&sigset);
        sigaddset(&sigset, SIG);

	// Use Real-Time scheduler, when available (needs root/setuid)
        retval = sched_setscheduler(0, SCHED_FIFO, &sched_param);
        if (0 != retval) {
        	perror("Failed to set RT scheduler");
        	if (EPERM == errno) {
        		puts("Resuming without RT scheduler");
        	} else {
        		exit(-1);
        	}
        }

	// Assign the custom signal handler
        if (SIG_ERR == signal(SIGINT, sighandler)) {
        	perror("Failed to assign SIGINT signal handler");
        	exit(-1);
        }

	// Create the timer
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIG;
	sev.sigev_value.sival_ptr = &timerid;
        retval = timer_create(CLOCKID, &sev, &timerid);
        if (0 != retval) {
        	perror("Failed to create new timer with timer_create()");
        	exit(-1);
        }

	unsigned long max = 0;
	unsigned long min = 1000000000UL;
	long period;

	struct timespec cur_time, prev_time;

	puts("Hit CTRL+C when you wish to quit...");

	// Start the timer and log the first timestamp
	timer_settime(timerid, 0, &tspec, NULL);
	clock_gettime(CLOCKID, &prev_time);
	int sig;

	/*
	 * Infinite loop that breaks on SIGINT, consumes some CPU while idle
	 * but significantly improves the jitter. Tried using mutex and
	 * semaphore but both degrade the jitter.
	 */
	sigprocmask(SIG_BLOCK, &sigset, NULL);
        while (keep_running) {
        	sigwait(&sigset, &sig);
        	
		clock_gettime(CLOCKID, &cur_time);
		period = cur_time.tv_nsec - prev_time.tv_nsec;
		// Compensate tv_nsec overflow
		if (period < 0)
			period += 1000000000UL;
		if (period < min)
			min = period;
		else if (period > max)
			max = period;
		prev_time = cur_time;
        }

	// Print the results
	printf("Max interval: %luns (+%luns)\n", max, max - 1000000UL);
	printf("Min interval: %luns (-%luns)\n", min, 1000000UL - min);
        printf("Jitter: %ldns\n", max - min);

        retval = timer_delete(timerid);
        if (0 != retval) {
        	perror("Failed to delete timer");
        	exit(-1);
        }

        return EXIT_SUCCESS;
}
