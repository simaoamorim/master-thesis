#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

static void * wait_q(void *arg);

int main (int argc, char *argv[])
{
        pthread_t thread_info;
        int retval = 0;
        puts("Going to create a thread");
        pthread_create(&thread_info, NULL, &wait_q, NULL);
        pthread_join(thread_info, (void *) &retval);
        printf("Thread exited with status %d\n", retval);
        return 0;
}

static void * wait_q(void *arg)
{
        puts("Hello world from a thread!");
        puts("Returning...");
        pthread_exit((void *) 0);
}
