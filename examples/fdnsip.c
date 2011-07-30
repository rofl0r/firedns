#include <stdio.h>
#include "../include/firedns.h"
#include "../../lib/include/strlib.h"

int main(int argc, char **argv) {
	struct in_addr *result;

	if (argc != 2) {
		ulz_fprintf(2,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}

	result = firedns_resolveip4(argv[1]);
	if (result) {
		ulz_printf("%s\n",firedns_ntoa4(result));
		return 0;
	}

	return 1;
}
