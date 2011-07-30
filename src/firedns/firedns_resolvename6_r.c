#include "firedns_internal.h"

char *firedns_resolvename6_r(const struct in6_addr * const ip) {
	return firedns_resolvename6_i(ip,firedns_getresult_r);
}

