#include <encoder.h>
#include <encoder_task.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>

int main (int argc, char *argv[])
{
	int retval;
	struct encoder enc = {0};
	if (-1 == encoder_init(&enc, 0, 17, 18)) {
		perror("Failed to open encoder GPIO");
		return -1;
	}
	pthread_t tid;
	pthread_create(&tid, NULL, encoder_task, &enc);
	for (int i = 0; i < 10; i++) {
		sleep(1);
		printf("Counter: %ld\n", encoder_task_get_count(&enc));
	}
	pthread_kill(tid, SIGINT);
	pthread_join(tid, (void *) &retval);
	printf("Return value: %d\n", retval);
	printf("Final stage: %d\n", enc.stage);
	printf("Counter: %ld\n", enc.count);
	return 0;
}