#include "firedns_internal.h"

struct in6_addr *firedns_resolveip6(firedns_state* self, const char * const name) {
	return firedns_resolveip6_i(self, name, firedns_getresult);
}

