#include <stdio.h>
#include <string.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	struct firedns_ip6list *iter;
	firedns_state dns, *d = &dns;
	char ntoabuf[256];

	if (argc != 2) {
		fprintf(stderr,"usage: %s <hostname>\n",argv[0]);
		return 2;
	}
	firedns_init(d);
	firedns_add_servers_from_resolv_conf(d);

	iter = firedns_resolveip6list(d, argv[1]);

	if (iter == NULL)
		return 1;

	while (iter != NULL) {
		printf("%s\n",firedns_ntoa6(&iter->ip, ntoabuf));
		iter = iter->next;
	}

	return 0;
}
