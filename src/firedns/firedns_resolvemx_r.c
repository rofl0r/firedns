#include "firedns_internal.h"

char *firedns_resolvemx_r(const char * const name) {
	return firedns_resolvemx_i(name,firedns_getresult_r);
}

