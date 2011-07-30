#include <stdio.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	struct in_addr *result;

	if (argc != 2) {
		fprintf(stderr,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}

	result = firedns_resolveip4(argv[1]);
	if (result) {
		printf("%s\n",firedns_ntoa4(result));
		return 0;
	}

	return 1;
}
