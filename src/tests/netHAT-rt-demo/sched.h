#ifndef sched_h_
#define sched_h_

#include <linux/sched/types.h>
#include <sys/syscall.h>
#include <unistd.h>

int sched_setattr(pid_t pid, const struct sched_attr *attr, unsigned int flags);

int sched_getattr(pid_t pid, struct sched_attr *attr, unsigned int size, unsigned int flags);

int sched_yield();

#endif
