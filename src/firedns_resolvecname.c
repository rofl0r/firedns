#include "firedns_internal.h"

char *firedns_resolvecname(firedns_state* self, const char * const name) {
	return firedns_resolvecname_i(self, name, firedns_getresult);
}

