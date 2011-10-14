#include "firedns_internal.h"

char *firedns_resolvetxt(firedns_state* self, const char* name) { 
	return firedns_resolvetxt_i(self, name, firedns_getresult);
}

