#include <stdio.h>
#include <wiringPi.h>

int main (int argc, char *argv[])
{
	if (-1 == wiringPiSetup()) {
		puts("Setup failed");
		return -1;
	}
	puts("Setup OK");
	return 0;
}
