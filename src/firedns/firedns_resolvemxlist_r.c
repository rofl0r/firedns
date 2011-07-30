#include "firedns_internal.h"

struct firedns_mxlist *firedns_resolvemxlist_r(const char * const name) {
	return firedns_resolvemxlist_i(name,firedns_getresult_r);
}

