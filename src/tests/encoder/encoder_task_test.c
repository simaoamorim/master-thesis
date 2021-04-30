#include <encoder.h>
#include <encoder_task.h>

#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#define ENCODER_PPR 12

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
	for (int i = 0; i < 10; i++) {
		sleep(1);
		count = encoder_task_get_count(&enc);
		printf("Counter: %ld\n", count);
		printf("Revolutions: %f\n", apply_scale(count, ENCODER_PPR));
	}
	pthread_kill(tid, SIGINT);
	pthread_join(tid, (void *) &retval);
	printf("Return value: %d\n", retval);
	printf("Final stage: %d\n", enc.encoder.stage);
	printf("Counter: %ld\n", enc.encoder.count);
	return 0;
}