#include <stdio.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	char *result;
	firedns_state dns, *d = &dns; 

	if (argc != 2) {
		fprintf(stderr,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}

	firedns_init(d);
	firedns_add_servers_from_resolve_conf(d);

	result = firedns_resolvecname(d, argv[1]);
	if (result) {
		printf("%s\n",result);
		return 0;
	}

	return 1;
}
