#include "firedns_internal.h"

struct firedns_mxlist *firedns_resolvemxlist(const char * const name) {
	return firedns_resolvemxlist_i(name,firedns_getresult);
}

