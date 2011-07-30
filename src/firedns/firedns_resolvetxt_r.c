#include "firedns_internal.h"

char *firedns_resolvetxt_r(const char * const name) {
	return firedns_resolvetxt_i(name,firedns_getresult_r);
}

