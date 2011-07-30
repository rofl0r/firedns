#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <unistd.h>
#include "../include/firemake.h"
#include "../include/firedns.h"

#define TESTS 10000
#define TEST_HOST "aol.com"

struct timeval tv;

void start_time() {
	if (gettimeofday(&tv,NULL) != 0) {
		perror("gettimeofday()");
		exit(102);
	}
}

double end_time() {
	struct timeval end;
	double timetaken;

	if (gettimeofday(&end,NULL) != 0) {
		perror("gettimeofday()");
		exit(102);
	}


	timetaken = end.tv_sec + end.tv_usec / 1000000.0;
	timetaken -= tv.tv_sec + tv.tv_usec / 1000000.0;

	printf("Delta seconds: %f\n",timetaken);

	return timetaken;
}

int main() {
	int i;
	double libc, fire;

	printf("Testing %d firedns_resolveip4list(\"%s\"):\n",TESTS,TEST_HOST);
	start_time();
	for (i = 0; i < TESTS; i++)
		firedns_resolveip4list(TEST_HOST);
	fire = end_time();

	printf("Testing %d gethostbyname(\"%s\"):\n",TESTS,TEST_HOST);
	start_time();
	for (i = 0; i < TESTS; i++)
		gethostbyname(TEST_HOST);
	libc = end_time();

	if (libc > fire)
		printf("Speedup: %d%%\n",(int) (((libc - fire) / libc) * 100.0));
	else
		printf("Slowdown: %d%%\n",(int) (((fire - libc) / libc) * 100.0));

	return 0;
}
