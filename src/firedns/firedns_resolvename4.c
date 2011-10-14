#include "firedns_internal.h"

char *firedns_resolvename4(firedns_state* self, const struct in_addr* ip) { 
	return firedns_resolvename4_i(self, ip, firedns_getresult);
}

