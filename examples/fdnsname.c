#include <stdio.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	char *result;
	struct in_addr *binip;
	struct in6_addr *binip6;
	firedns_state dns, *d = &dns;

	if (argc != 2) {
		fprintf(stderr,"usage: %s <ip address>\n",argv[0]);
		return 2;
	}
	
	firedns_init(d);
	
	binip6 = firedns_aton6(argv[1]);
	if (binip6 == NULL) {
		binip = firedns_aton4(argv[1]);
		if (binip == NULL) {
			fprintf(stderr,"invalid IP address.\n");
			return 2;
		}

		result = firedns_resolvename4(d, binip);
	} else
		result = firedns_resolvename6(d, binip6);

	if (result) {
		printf("%s\n",result);
		return 0;
	}

	return 1;
}
