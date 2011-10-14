#include "firedns_internal.h"

struct firedns_ip4list *firedns_resolveip4list(firedns_state* self, const char* name) { 
	return firedns_resolveip4list_i(self, name, firedns_getresult);
}

