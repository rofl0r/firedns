#include "firedns_internal.h"

struct firedns_mxlist *firedns_resolvemxlist(firedns_state* self, const char* name) {
	return firedns_resolvemxlist_i(self, name,firedns_getresult);
}

