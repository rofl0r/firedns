#include <stdio.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	struct in6_addr *result;
	firedns_state dns, *d = &dns;
	char ntoabuf[256];

	if (argc != 2) {
		fprintf(stderr,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}
	firedns_init(d);
	result = firedns_resolveip6(d, argv[1]);
	if (result) {
		printf("%s\n",firedns_ntoa6(result, ntoabuf));
		return 0;
	}

	return 1;
}
