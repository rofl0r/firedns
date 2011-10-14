#include <stdio.h>
#include "../include/firedns.h"
#include "../../lib/include/strlib.h"

int main(int argc, char **argv) {
	struct in_addr *result;
	firedns_state dns, *d = &dns;

	if (argc != 2) {
		ulz_fprintf(2,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}
	firedns_init(d);
	result = firedns_resolveip4(d, argv[1]);
	if (result) {
		ulz_printf("%s\n",firedns_ntoa4(result));
		return 0;
	}

	return 1;
}
