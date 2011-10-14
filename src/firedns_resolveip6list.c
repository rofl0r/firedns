#include "firedns_internal.h"

struct firedns_ip6list *firedns_resolveip6list(firedns_state* self, const char* name) { 
	return firedns_resolveip6list_i(self, name,firedns_getresult);
}

