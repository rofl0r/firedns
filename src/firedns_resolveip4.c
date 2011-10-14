#include "firedns_internal.h"

struct in_addr *firedns_resolveip4(firedns_state* self, const char * const name) { 
	return firedns_resolveip4_i(self, name, firedns_getresult);
}

