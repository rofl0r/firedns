#include "firedns_internal.h"
#include <string.h>

/* returns 0 on success, else the number of fails (2 possible, one for ipv6, one for ipv4 */
int firedns_add_server(firedns_state* self, char* server) {
	int fail = 0;
	struct in_addr addr4;
#ifdef HAVE_IPV6
	struct in6_addr addr6;

	if (self->i6 < FDNS_MAX && firedns_aton6(server, &addr6) != NULL)
		memcpy(&self->servers6[(self->i6)++], &addr6, sizeof(struct in6_addr));
	else
		fail++;
#endif
	if (self->i4 < FDNS_MAX && firedns_aton4(server, &addr4) != NULL)
		memcpy(&self->servers4[(self->i4)++], &addr4, sizeof(struct in_addr));
	else
		fail++;

	return fail;
}

