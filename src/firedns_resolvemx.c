#include "firedns_internal.h"

char *firedns_resolvemx(firedns_state* self, const char* name) { 
	return firedns_resolvemx_i(self, name, firedns_getresult);
}

