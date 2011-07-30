#include "firedns_internal.h"

char *firedns_resolvecname(const char * const name) {
	return firedns_resolvecname_i(name,firedns_getresult);
}

