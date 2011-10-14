#include "firedns_internal.h"

char *firedns_resolvename6(firedns_state* self, const struct in6_addr* ip) {
	return firedns_resolvename6_i(self, ip,firedns_getresult);
}

