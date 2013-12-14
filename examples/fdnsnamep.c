#define _GNU_SOURCE /* strdup() */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/firedns.h"

int main(int argc, char **argv) {
	char *result;
	struct in6_addr buf6;
	struct in_addr buf4;
	struct in_addr *binip;
	struct in6_addr *binip6;
	struct firedns_ip4list *ipiter;
	struct firedns_ip6list *ip6iter;
	char *local_result;
	firedns_state dns, *d = &dns;

	if (argc != 2) {
		fprintf(stderr,"usage: %s <ip address>\n",argv[0]);
		return 2;
	}
	firedns_init(d);
	firedns_add_servers_from_resolv_conf(d);

	binip6 = firedns_aton6(argv[1], &buf6);
	if (binip6 == NULL) {
		binip = firedns_aton4(argv[1], &buf4);
		if (binip == NULL) {
			fprintf(stderr,"invalid IP address.\n");
			return 2;
		}

		/* IPv4 */
		result = firedns_resolvename4(d, binip);

		if (result == NULL)
			return 1;

		local_result = strdup(result);

		ipiter = firedns_resolveip4list(d, result);
		while (ipiter != NULL) {
			if (memcmp(&ipiter->ip,binip,4) == 0)
				goto good;
			ipiter = ipiter->next;
		}

		free(local_result);
		return 3;
	} else {
		/* IPv6 */
		result = firedns_resolvename6(d, binip6);

		if (result == NULL)
			return 1;

		local_result = strdup(result);

		ip6iter = firedns_resolveip6list(d, result);
		while (ip6iter != NULL) {
			if (memcmp(&ip6iter->ip,binip6,16) == 0)
				goto good;
			ip6iter = ip6iter->next;
		}

		free(local_result);
		return 3;
	}

good:
	printf("%s\n",local_result);
	free(local_result);
	return 0;
}
