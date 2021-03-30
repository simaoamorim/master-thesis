/*
 * rt-task.c - Real-time task testing
 *
 * Copyright (c) 2021 Simão Amorim <simao.amorim@fe.up.pt>
 *
 * This program creates a periodic task running on an infinite loop. The period
 * is provided by the kernel's `timer' functions. For more information see the
 * manpage timer_create(2). The signal-backed timer is used on this example, so
 * a custom signal handler function is used.
 *
 * This example includes some advanced features as setting the scheduler used
 * for this program's thread to be the real-time FIFO scheduler and setting the
 * CPU affinity, only allowing the thread to run of the specified CPU (CPU0).
 *
 * This source code is licensed under the MIT license.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

// needed for sched_setaffinity and CPU functions
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sched.h>

/********************/
/* Global Variables */
/********************/

volatile int mutex = 0;
volatile int keep_running = 1;

/******************/
/* Signal Handler */
/******************/

void sighandler(int signum) {
	if (SIGALRM == signum) {
		mutex = 1;
	} else if (SIGINT == signum) {
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

	// Lock the process to CPU 0 only
        cpu_set_t cpu_set;
        CPU_ZERO(&cpu_set);
        CPU_SET(0, &cpu_set);
        retval = sched_setaffinity(0, sizeof(cpu_set), &cpu_set);
        if (0 != retval) {
        	perror("Failed to set cpu affinity");
        	exit(-1);
        }

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
        if (SIG_ERR == signal(SIGALRM, sighandler)) {
        	perror("Failed to assign SIGALRM signal handler");
        	exit(-1);
        }
        if (SIG_ERR == signal(SIGINT, sighandler)) {
        	perror("Failed to assign SIGINT signal handler");
        	exit(-1);
        }

	// Create the timer
        retval = timer_create(CLOCK_MONOTONIC, NULL, &timerid);
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
	clock_gettime(CLOCK_MONOTONIC, &prev_time);

	/*
	 * Infinite loop that breaks on SIGINT, consumes some CPU while idle
	 * but significantly improves the jitter. Tried using mutex and
	 * semaphore but both degrade the jitter.
	 */
        while (keep_running) {
        	if (0 == mutex) {
        		usleep(100);
        	} else {
        		mutex = 0;
        		clock_gettime(CLOCK_MONOTONIC, &cur_time);
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
