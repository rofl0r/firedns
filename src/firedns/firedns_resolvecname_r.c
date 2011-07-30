#include "firedns_internal.h"

char *firedns_resolvecname_r(const char * const name) {
	return firedns_resolvecname_i(name,firedns_getresult_r);
}

