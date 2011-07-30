#include "firedns_internal.h"

struct in6_addr *firedns_resolveip6(const char * const name) {
	return firedns_resolveip6_i(name,firedns_getresult);
}

