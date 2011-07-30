#include <stdio.h>
#include <string.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	char *result;

	if (argc != 2) {
		fprintf(stderr,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}

	result = firedns_resolvemx(argv[1]);

	if (result != NULL) {
		printf("%s\n",result);
		return 0;
	}

	return 1;
}
