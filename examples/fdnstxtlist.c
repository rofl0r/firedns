#include <stdio.h>
#include <string.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	struct firedns_txtlist *iter;
	firedns_state dns, *d = &dns;

	if (argc != 2) {
		fprintf(stderr,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}

	firedns_init(d);
	firedns_add_servers_from_resolve_conf(d);

	iter = firedns_resolvetxtlist(d, argv[1]);

	if (iter == NULL)
		return 1;

	while (iter != NULL) {
		printf("%s\n",iter->txt);
		iter = iter->next;
	}

	return 0;
}
