#include <stdio.h>
#include <stdlib.h>

#include <signal.h>
#include <time.h>

int main(int argc, char *argv[])
{
        struct sigevent sigevent = {};
        timer_t timerid;

        timer_create(CLOCK_MONOTONIC, &sigevent, &timerid);

        return EXIT_SUCCESS;
}
