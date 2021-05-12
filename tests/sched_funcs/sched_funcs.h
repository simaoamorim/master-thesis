#ifndef _SCHED_FUNCS_H_
#define _SCHED_FUNCS_H_

#include <unistd.h>
#include <syscall.h>
#include <linux/sched.h>
#include <linux/sched/types.h>

int sched_setattr(pid_t pid, struct sched_attr *attr, unsigned int flags);

int sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags);

int sched_yield();

#endif
